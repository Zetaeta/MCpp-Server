
#ifndef MINECRAFTSERVER_HPP
#define MINECRAFTSERVER_HPP

#include <string>
#include <map>
#include <streambuf>
#include <Util/stlfwd.hpp>


#ifndef HEADER_OPENSSL_TYPES_H
typedef struct rsa_st RSA;
#endif

namespace MCServer { 
namespace Logging {
class Logger;
}

namespace Network {
class NetworkServer;
}

namespace UI {
class UIManager;
}

class EntityManager;
namespace Plugin {
class PluginManager;
}

namespace Command {
class CommandManager;
}

namespace Entities {
class Player;
};

namespace Events {
class EventManager;
}

class World;
class Scheduler;
class ChatServer;
class ChunkLoader;
class CommandSender;
class ServerCommandSender;

enum GameMode : unsigned char;
enum Difficulty : unsigned char;
enum WorldType : signed char;

struct MinecraftServerData;

class MinecraftServer {
public:
    MinecraftServer(const std::map<std::string, std::string *> &, int &argc, char **argv);
    ~MinecraftServer();
    void run();
    void shutdown();

    bool isRunning();
    bool isShutdown();

    int & argc();
    char **argv();
    std::streambuf * getStdout();
    std::streambuf * getStderr();
    Logging::Logger & getLogger();
    Network::NetworkServer & getNetworkServer();
    UI::UIManager & getUIManager();
    EntityManager & getEntityManager();
    Plugin::PluginManager & getPluginManager();
    Scheduler & getScheduler();
    Command::CommandManager & getCommandManager();
    ChatServer & getChatServer();
    ChunkLoader & getChunkLoader();
    Events::EventManager & getEventManager();
    std::shared_ptr<CommandSender> getCommandSender();
    const std::shared_ptr<ServerCommandSender> & getServerCommandSender();

    std::string getVersion();
    std::string getMotd();
    int getMaxPlayers();
    int getOnlinePlayerCount();
    bool userValidationEnabled() {
        return false;
    }
    std::string getLevelType();
    GameMode getDefaultGameMode();
    WorldType getWorldType();
    Difficulty getDifficulty();
    World & getWorld(int);
    World & getMainWorld();
    std::vector<World *> getWorlds();
    
    std::string getSetting(const std::string &);

    RSA * getRsa();
    std::string getAsn1PublicKey();
    std::string getVerifyToken();
    std::string getServerId();
    std::string getPublicKey();
    const std::vector<std::shared_ptr<Entities::Player>> & getPlayers();

    void dispatchConsoleCommand(const std::string &command,
                                const std::shared_ptr<CommandSender> &);
    void addPlayer(std::shared_ptr<Entities::Player> &);
    void removePlayer(std::shared_ptr<Entities::Player> &);
    
    static inline MinecraftServer & getServer() {
        return *instance;
    }

    static inline MinecraftServer & server() {
        return *instance;
    }

    static MinecraftServer *instance;

protected:
    MinecraftServerData *m; // PIMPL

    void initUI();
    void init();
    void tick();
    void setupWorlds();
    void loadWorld(const std::string &directory);

private:
    // Inaccessible copy constructor.
    MinecraftServer(const MinecraftServer &);
};

} /* namespace MCServer */
#endif /* MINECRAFTSERVER_HPP_ */
