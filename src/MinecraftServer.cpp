/*
 * MinecraftServer.cpp
 *
 *  Created on: 3 Aug 2012
 *      Author: daniel
 */

#include <iostream>
#include <exception>
#include <cstdlib>

#include <unistd.h>

#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <openssl/rc4.h>

#include "MinecraftServer.hpp"
#include "network/NetworkServer.hpp"
#include "logging/Logger.hpp"
#include "network/PacketHandler.hpp"
#include "GameMode.hpp"
#include "WorldType.hpp"
#include "Difficulty.hpp"
#include "EntityManager.hpp"
#include "plugin/PluginManager.hpp"
#include "ui/UIManager.hpp"
#include "util/StringUtils.hpp"
#include "util/Utils.hpp"

namespace MCServer {

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::map;

using Network::NetworkServer;
using Network::PacketHandler;
using Logging::Logger;
using Logging::Level;
USING_LOGGING_LEVEL
using Plugins::PluginManager;
using UI::UIManager;

#ifndef PLUGIN_DIR
#define PLUGIN_DIR "plugins/"
#endif

struct MinecraftServerData {
    bool shutdown;
    map<string, string *> options;

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
    

    static MinecraftServer *server;
};
MinecraftServer * MinecraftServerData::server;

namespace {
void shutdownServer() {
    if (&MinecraftServer::getServer() && !MinecraftServer::getServer().isShutdown()) {
        MinecraftServer::getServer().shutdown();
    }
}
}

MinecraftServer::MinecraftServer(const map<string, string *> &options)
:m(new MinecraftServerData()) {
    atexit(&shutdownServer);
    MinecraftServerData::server = this;
    m->options = options;

    initUI();

    m->logger = &Logger::getLogger("Minecraft");
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
        cerr << "Unhandled exception in server: " << e.what() << endl;
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

}

void MinecraftServer::shutdown() {
    m->shutdown = true;
}

bool MinecraftServer::isShutdown() {
    return m->shutdown;
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


void MinecraftServer::dispatchConsoleCommand(string command) {
    m->logger->info("Dispatching command " + command);
}


MinecraftServer & MinecraftServer::getServer() {
    return *MinecraftServerData::server;
}

} /* namespace MCServer */
