/*
 * MinecraftServer.hpp
 *
 *  Created on: 3 Aug 2012
 *      Author: daniel
 */

#ifndef MINECRAFTSERVER_HPP
#define MINECRAFTSERVER_HPP

#include <string>
#include <map>

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
namespace Plugins {
class PluginManager;
}

enum GameMode : unsigned char;
enum Difficulty : unsigned char;
enum WorldType : signed char;

struct MinecraftServerData;

class MinecraftServer {
public:
    MinecraftServer(const std::map<std::string, std::string *> &);
    ~MinecraftServer();
    void run();
    void init();
    void tick();
    void shutdown();

    bool isShutdown();

    Logging::Logger & getLogger();
    Network::NetworkServer & getNetworkServer();
    UI::UIManager & getUIManager();
    EntityManager & getEntityManager();
    Plugins::PluginManager & getPluginManager();

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

    void dispatchConsoleCommand(std::string command);
    
    static MinecraftServer & getServer();
protected:
    MinecraftServerData *m; // PIMPL

    void initUI();
};

} /* namespace MCServer */
#endif /* MINECRAFTSERVER_HPP_ */
