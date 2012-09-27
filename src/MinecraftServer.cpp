
#include <iostream>
#include <exception>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <typeinfo>
#include <cxxabi.h>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <openssl/rc4.h>

#include "MinecraftServer.hpp"
#include "int128.h"


#include "logging/Logger.hpp"
#include "logging/LoggerStreamBuf.hpp"

#include "network/NetworkServer.hpp"
#include "network/PacketHandler.hpp"

#include "plugin/PluginManager.hpp"
#include "ui/UIManager.hpp"

#include "game/GameMode.hpp"
#include "game/WorldType.hpp"
#include "game/Difficulty.hpp"
#include "game/EntityManager.hpp"
#include "game/World.hpp"
#include "game/WorldLoadingFailure.hpp"

#include "util/StringUtils.hpp"
#include "util/Utils.hpp"
#include "util/FSUtils.hpp"

namespace MCServer {

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::streambuf;

using Network::NetworkServer;
using Network::PacketHandler;
using Logging::Logger;
using Logging::Level;
using Logging::LoggerStreamBuf;
USING_LOGGING_LEVEL
using Plugin::PluginManager;
using UI::UIManager;

#ifndef PLUGIN_DIR
#define PLUGIN_DIR "plugins/"
#endif

MinecraftServer *MinecraftServer::_server = 0;

struct MinecraftServerData {
    bool shutdown;
    map<string, string *> options;
    int &argc;
    char **argv;

    streambuf *realStdout;
    streambuf *realStderr;

    NetworkServer *networkServer;
    Logger *logger;
    EntityManager *entityManager;
    PluginManager *pluginManager;
    UIManager *uiManager;

    // Crypto stuff
    RSA *rsa;
    X509 *x509;
    EVP_PKEY *pk;
    string serverId;
    string publicKey;
    string verifyToken;

    map<string, World> worlds;

    MinecraftServerData(int &argc)
    :argc(argc) {}
};

namespace {
void shutdownServer() {
    if (&MinecraftServer::getServer() && !MinecraftServer::getServer().isShutdown()) {
        MinecraftServer::getServer().shutdown();
    }
}
}

MinecraftServer::MinecraftServer(const map<string, string *> &options, int &argc, char **argv)
:m(new MinecraftServerData(argc)) {

    _server = this;
    m->argv = argv;
    atexit(&shutdownServer);
    m->options = options;
    m->realStdout = cout.rdbuf();
    m->realStderr = cerr.rdbuf();

    initUI();

    m->logger = &Logger::getLogger("Minecraft");
    cout.rdbuf(new LoggerStreamBuf(*m->logger, INFO));
    cerr.rdbuf(new LoggerStreamBuf(*m->logger, SEVERE));
    
    m->entityManager = new EntityManager(this);
    m->pluginManager = new PluginManager(this);

    m->rsa = RSA_generate_key(1024, 17, 0, 0);

    m->x509 = X509_new();
    m->pk = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(m->pk, m->rsa);
    X509_set_version(m->x509, 0);
    X509_set_pubkey(m->x509, m->pk);

    int len;
    uint8_t *buf = NULL;
    len = i2d_X509(m->x509, &buf);
    
    // Wat.
    m->publicKey = string(reinterpret_cast<char *>(buf) + 28, len - 36);
    OPENSSL_free(buf);
    
    string encryptionByteCharacters = "abcedfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=";
    string hexDigits = "0123456789abcdef";
    char serverId_[17];
    char *serverId = serverId_;
    char verifyToken[5];
    for (int i=0; i<16; ++i) { // Server ID random generation
        serverId[i] = hexDigits[rand() % hexDigits.size()];
    }
    serverId[16] = '\0';
    for (int i=0; i<4; ++i) { // Encryption bytes random generation.
        verifyToken[i] = encryptionByteCharacters[rand() % encryptionByteCharacters.size()];
    }
    verifyToken[4] = '\0';
    if (!userValidationEnabled()) {
        serverId = "-";
    }
    *m->logger << INFO << "Server ID: " << serverId << '\n';
    m->serverId = serverId;
    m->verifyToken = verifyToken;

    PacketHandler::initialise(this);

}

MinecraftServer::~MinecraftServer() {
    if (!m->shutdown) {
        shutdown();
    }
    delete m;
}

void MinecraftServer::run() {
    try {
        init();
        while (true) {
            tick();
        }
    }
    catch (std::exception &e) {
        cerr << "Unhandled exception in server: " << demangle(typeid(e).name()) << ": "<< e.what() << endl;
    }
    catch (const char *str) {
        cerr << "Unhandled (const char *) exception in server: " << str << endl;
    }
    catch (std::string &str) {
        cerr << "Unhandled (string) exception in server: " << str << endl;
    }

    try {
        shutdown();
    }
    catch (std::exception &e) {
        cerr << "Error shutting down: " << e.what() << endl;
        exit(1);
    }
    catch (const char *str) {
        cerr << "Error shutting down: " << str << endl;
        exit(1);
    }
    catch (std::string &str) {
        cerr << "Error shutting down: " << str << endl;
        exit(1);
    }
    
    
}

void MinecraftServer::init() {
    m->logger->info("Starting MC++-Server...");
    m->logger->info("Server version: " + getVersion());
    m->pluginManager->loadPlugins(PLUGIN_DIR);
    m->networkServer = new NetworkServer(this);
    setupWorlds();
}

void MinecraftServer::initUI() {
    map<string, string *>::iterator gui = m->options.find("gui");
    map<string, string *>::iterator ui = m->options.find("ui");
    map<string, string *>::iterator cli = m->options.find("cli");

    // Check for more than one UI option
    int optCount(0);
    if (gui != m->options.end())
        ++optCount;
    if (ui != m->options.end())
        ++optCount;
    if (cli != m->options.end())
        ++optCount;
    if (optCount > 1) {
        invalidOption("Can only specify one of --cli, --gui and --ui!");
    }

    std::string uiName;
    if (ui != m->options.end()) {
        uiName = toLower(*ui->second);
        if (uiName == "gui") {
            uiName = "qt";
        }
    }
    else if (gui != m->options.end()) {
        uiName = "qt";
    }
    else if (cli != m->options.end()) {
        uiName = "cli";
    }
    else {
        uiName = "cli";
    }
    m->uiManager = &UIManager::init(this, uiName);
}

void MinecraftServer::tick() {
//    sleep(1);
//    *m->logger << "Testing m->logger!\n";
//    std::cout << "Testing cout!\n";
}

void MinecraftServer::shutdown() {
    m->shutdown = true;
    exit(0);
}

bool MinecraftServer::isShutdown() {
    return m->shutdown;
}

namespace {

struct isntWorldFolder {
    bool operator()(const std::string &fileName) {
        return !exists(fileName + "/level.dat");
    }
};
}

void MinecraftServer::setupWorlds() {
    vector<string> entries;
    getEntries(".", entries);
    if (!inUsrShare()) {
        getEntries("/usr/share/mc++-server", entries);
    }
    vector<string>::iterator end = std::remove_if(entries.begin(), entries.end(), isntWorldFolder());

    for (vector<string>::iterator it = entries.begin(); it != end; ++it) {
        *m->logger << "Going to load " << *it << '\n';
        loadWorld(*it);
    }
}

void MinecraftServer::loadWorld(const std::string &directory) {

    World world;
    try {
        world.loadFrom(directory);
    } catch (const WorldLoadingFailure &ex) {
        *m->logger << "Failed loading world in " << directory << ": " << ex.what() << '\n';
        return;
    }
    *m->logger << "Loaded world!\n";
    m->worlds[world.getName()] = std::move(world);

}


int & MinecraftServer::argc() {
    return m->argc;
}

char ** MinecraftServer::argv() {
    m->argv;
}

streambuf * MinecraftServer::getStdout() {
    return m->realStdout;
}

streambuf * MinecraftServer::getStderr() {
    return m->realStderr;
}

NetworkServer & MinecraftServer::getNetworkServer() {
    return *m->networkServer;
}

Logger & MinecraftServer::getLogger() {
    return *m->logger;
}

UIManager & MinecraftServer::getUIManager() {
    return *m->uiManager;
}

EntityManager & MinecraftServer::getEntityManager() {
    return *m->entityManager;
}


string MinecraftServer::getVersion() {
    return "0.1";
}

std::string MinecraftServer::getMotd() {
    return "MC++-Server. Meow";
}

int MinecraftServer::getMaxPlayers() {
    return 1024;
}

int MinecraftServer::getOnlinePlayerCount() {
    return 2048;
}

string MinecraftServer::getServerId() {
    return m->serverId;
}

string MinecraftServer::getLevelType() {
    return "default";
}

GameMode MinecraftServer::getDefaultGameMode() {
    return CREATIVE;
}

WorldType MinecraftServer::getWorldType() {
    return OVERWORLD;
}

Difficulty MinecraftServer::getDifficulty() {
    return PEACEFUL;
}


string MinecraftServer::getPublicKey() {
    return m->publicKey;
}

string MinecraftServer::getVerifyToken() {
    return m->verifyToken;
}

RSA * MinecraftServer::getRsa() {
    return m->rsa;
}


void MinecraftServer::dispatchConsoleCommand(const string &command) {
    m->logger->info("Dispatching command " + command);
}

} /* namespace MCServer */
