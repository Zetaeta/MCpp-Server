
#ifndef MINECRAFTSERVER_HPP
#define MINECRAFTSERVER_HPP

#include <string>
#include <map>
#include <streambuf>


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

    std::string getVersion();
    std::string getMotd();
    int getMaxPlayers();
    int getOnlinePlayerCount();
    bool userValidationEnabled() {return true;}
    std::string getLevelType();
    GameMode getDefaultGameMode();
    WorldType getWorldType();
    Difficulty getDifficulty();
    
    std::string getSetting(const std::string &);

    RSA * getRsa();
    std::string getAsn1PublicKey();
    std::string getVerifyToken();
    std::string getServerId();
    std::string getPublicKey();

    void dispatchConsoleCommand(const std::string &command);
    
    static inline MinecraftServer & getServer() {
        return *_server;
    }

    static inline MinecraftServer & server() {
        return *_server;
    }

protected:
    MinecraftServerData *m; // PIMPL

    void initUI();
    void init();
    void tick();
    void setupWorlds();
    void loadWorld(const std::string &directory);

private:
    static MinecraftServer *_server;
    // Inaccessible copy constructor.
    MinecraftServer(const MinecraftServer &);
};

} /* namespace MCServer */
#endif /* MINECRAFTSERVER_HPP_ */
