
#include <iostream>
#include <exception>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <typeinfo>
#include <string.h>
#include <mcheck.h>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>

#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <openssl/rc4.h>

#include <Util/StringUtils.hpp>

#include "MinecraftServer.hpp"
#include "int128.h"
#include "Scheduler.hpp"
#include "ChatServer.hpp"
#include "ServerCommandSender.hpp"


#include "logging/Logger.hpp"
#include "logging/LoggerStreamBuf.hpp"
#include "logging/UIHandler.hpp"

#include "network/NetworkServer.hpp"
#include "network/PacketHandler.hpp"
#include "network/ClientConnection.hpp"

#include "plugin/PluginManager.hpp"
#include "ui/UIManager.hpp"

#include "game/ChunkLoader.hpp"
#include "game/GameMode.hpp"
#include "game/WorldType.hpp"
#include "game/Difficulty.hpp"
#include "game/EntityManager.hpp"
#include "game/World.hpp"
#include "game/WorldLoadingFailure.hpp"
#include "game/entity/Entity.hpp"
#include "game/entity/Player.hpp"
#include "game/ChunkCoordinates.hpp"
#include "game/Point3D.hpp"

#include "util/Utils.hpp"
#include "util/FSUtils.hpp"

#include "command/CommandManager.hpp"

#include "event/EventManager.hpp"
#include "event/Event.hpp"
#include "event/entity/player/PlayerChatEvent.hpp"

namespace MCServer {

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::streambuf;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::pair;
using std::make_shared;

using Util::demangle;
using Util::toLower;

using Network::NetworkServer;
using Network::PacketHandler;
using Logging::Logger;
using Logging::Level;
using Logging::LoggerStreamBuf;
USING_LOGGING_LEVEL
using Plugin::PluginManager;
using UI::UIManager;
using Entities::Entity;
using Entities::Player;
using Command::CommandManager;
using Events::EventManager;
using Events::Event;
using Events::PlayerChatEvent;

#ifndef PLUGIN_DIR
#define PLUGIN_DIR "plugins/"
#endif

MinecraftServer *MinecraftServer::instance = 0;

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
    Scheduler *scheduler;
    CommandManager *commandManager;
    ChatServer *chatServer;
    ChunkLoader *chunkLoader;
    EventManager *eventManager;
    shared_ptr<ServerCommandSender> serverCommandSender;

    // Crypto stuff
    RSA *rsa;
    X509 *x509;
    EVP_PKEY *pk;
    string serverId;
    string publicKey;
    string verifyToken;

    map<string, World *> worlds;
    map<int, World *> worldsByDimension;
    vector<shared_ptr<Player>> players;

    MinecraftServerData(int &argc)
    :shutdown(false), argc(argc) {}
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

    instance = this;
    m->argv = argv;
    atexit(&shutdownServer);
    m->options = options;
    m->realStdout = cout.rdbuf();
    m->realStderr = cerr.rdbuf();
    signal(SIGPIPE, SIG_IGN);

    initUI();

    m->logger = &Logger::getLogger("Minecraft");
    m->logger->addHandler(new Logging::UIHandler(this));
    cout.rdbuf(new LoggerStreamBuf(*m->logger, INFO));
    cerr.rdbuf(new LoggerStreamBuf(*m->logger, SEVERE));
    Logging::log = *m->logger;
    
    m->entityManager = new EntityManager(this);
    m->pluginManager = new PluginManager(this);
    m->commandManager = new CommandManager();
    m->chatServer = new ChatServer;
    m->chunkLoader = new ChunkLoader();
    m->eventManager = new EventManager;
    m->serverCommandSender = make_shared<ServerCommandSender>();

    int schedulerThreadCount = 4;
    auto stcit = options.find("scheduler-max-thread-count");
    if (stcit != options.end()) {
        schedulerThreadCount = atoi(stcit->second->c_str());
    }
    m->scheduler = new Scheduler(schedulerThreadCount);

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
    char serverId[17];
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
        strcpy(serverId, "-");
    }
    *m->logger << INFO << "Server ID: " << serverId << '\n';
    m->serverId = serverId;
    m->verifyToken = verifyToken;

    auto it = m->options.find("mtrace");
    if (it != m->options.end()) {
        setenv("MALLOC_TRACE", m->options["mtrace"]->c_str(), 1);
        mtrace();
    }

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

    m->commandManager->registerCommand("meow", [] (const string &cmd, const shared_ptr<CommandSender> &sender, const vector<string> &args) {
        sender->sendMessage("Hello there! I'm a cat too!");
        std::ostringstream oss;
        oss << "Your command was: " << cmd << "!";
        sender->sendMessage(oss.str());
    });

    m->commandManager->registerCommand("chunk", [] (const string &cmd, const shared_ptr<CommandSender> &sender, const vector<string> &args) {
        auto player = dynamic_pointer_cast<Player>(sender);
        if (!player) {
            return;
        }
        std::ostringstream oss;
        auto coords = static_cast<ChunkCoordinates>(player->getPosition());
        oss << "You are in chunk {" << coords.x << ", " << coords.z << "}.";
        sender->sendMessage(oss.str());
    });

    m->eventManager->registerListener(Events::EventType::PLAYER_CHAT, [] (Event &event) {
        auto evt = dynamic_cast<PlayerChatEvent &>(event);
        cout << "Chat event!\n" << evt.getPlayer()->getName() << "said " << evt.getMessage() << '\n';
    });

    m->chunkLoader->start();
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
    sleep(1);
    m->scheduler->tick();

    std::for_each(m->players.begin(), m->players.end(), [] (shared_ptr<Player> player) {
        try {
            player->sendKeepAlive();
        }
        catch (...) {
            cout << "Disconnecting player " << player->getName() << '\n';
            player->getConnection().shutdown();
        }
    });

    // TODO: Entity updating, physics handling etc.
}

void MinecraftServer::shutdown() {
    m->shutdown = true;
    m->networkServer->shutdown();
    exit(0);
}

bool MinecraftServer::isShutdown() {
    return m->shutdown;
}

bool MinecraftServer::isRunning() {
    return !m->shutdown;
}

namespace {

struct isntWorldFolder {
    bool operator()(const std::string &fileName) {
        return !exists(fileName + "/level.dat");
    }
};
}

void MinecraftServer::setupWorlds() {
/*    vector<string> entries;
    getEntries(".", entries);
    if (!inUsrShare()) {
        getEntries("/usr/share/mc++-server", entries);
    }
    vector<string>::iterator end = std::remove_if(entries.begin(), entries.end(), isntWorldFolder());

    for (vector<string>::iterator it = entries.begin(); it != end; ++it) {
        *m->logger << "Going to load " << *it << '\n';
        loadWorld(*it);
    }
    */
    loadWorld("world");
}

void MinecraftServer::loadWorld(const std::string &directory) {

    World *world = new World;
    try {
        world->loadFrom(directory);
    } catch (const WorldLoadingFailure &ex) {
        *m->logger << "Failed loading world in " << directory << ": " << ex.what() << '\n';
        return;
    }
    *m->logger << "Loaded world!\n";
    m->worlds[world->getName()] = world;
    m->worldsByDimension[world->getDimension()] = world;

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


int & MinecraftServer::argc() {
    return m->argc;
}

char ** MinecraftServer::argv() {
    return m->argv;
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

Scheduler & MinecraftServer::getScheduler() {
    return *m->scheduler;
}

CommandManager & MinecraftServer::getCommandManager() {
    return *m->commandManager;
}

ChatServer & MinecraftServer::getChatServer() {
    return *m->chatServer;
}

EventManager & MinecraftServer::getEventManager() {
    return *m->eventManager;
}

shared_ptr<CommandSender> MinecraftServer::getCommandSender() {
    return m->serverCommandSender;
}

const shared_ptr<ServerCommandSender> & MinecraftServer::getServerCommandSender() {
    return m->serverCommandSender;
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
//    return m->serverId;
    return "-";
}

string MinecraftServer::getLevelType() {
    return "default";
}

GameMode MinecraftServer::getDefaultGameMode() {
    return SURVIVAL;
}

WorldType MinecraftServer::getWorldType() {
    return OVERWORLD;
}

Difficulty MinecraftServer::getDifficulty() {
    return PEACEFUL;
}

World & MinecraftServer::getWorld(int dimension) {
    return *m->worldsByDimension[dimension];
}

World & MinecraftServer::getMainWorld() {
    return *m->worlds["world"];
}

vector<World *> MinecraftServer::getWorlds() {
    vector<World *> returned;
    for (pair<int, World *> p : m->worldsByDimension) {
        returned.push_back(p.second);
    }
    return returned;
}

const vector<shared_ptr<Player>> & MinecraftServer::getPlayers() {
    return m->players;
}


void MinecraftServer::dispatchConsoleCommand(const string &command,
                                             const std::shared_ptr<CommandSender> &sender) {
    m->commandManager->dispatchCommand(command, sender);
}

void MinecraftServer::addPlayer(shared_ptr<Player> &player) {
    m->players.push_back(player);
}

void MinecraftServer::removePlayer(shared_ptr<Player> &player) {
    auto it = find(m->players.begin(), m->players.end(), player);
    if (it != m->players.end()) {
        m->players.erase(it);
    }
}


} /* namespace MCServer */
