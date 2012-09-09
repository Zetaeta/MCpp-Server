/*
 * MinecraftServer.hpp
 *
 *  Created on: 3 Aug 2012
 *      Author: daniel
 */

#ifndef MINECRAFTSERVER_HPP_
#define MINECRAFTSERVER_HPP_

//#include <boost/asio/io_service.hpp>
#include <string>

#ifndef HEADER_OPENSSL_TYPES_H
struct RSA;
#endif

namespace MCServer {

#ifndef LOGGING_HPP
namespace Logging {
class Logger;
}
#endif

#ifndef NETWORKSERVER_HPP
namespace Network {
class NetworkServer;
}
#endif

#ifndef CONSOLEREADER_HPP
class ConsoleReader;
#endif

struct MinecraftServerData;

class MinecraftServer {
public:
    MinecraftServer();
    ~MinecraftServer();
    void run();
    void init();
    void tick();
    void shutdown();
    std::string getVersion();
    Logging::Logger & getLogger();
    Network::NetworkServer & getNetworkServer();
    ConsoleReader & getConsoleReader();
    void dispatchConsoleCommand(std::string command);
    std::string getMotd();
    int getMaxPlayers();
    int getOnlinePlayerCount();
    bool userValidationEnabled() {return true;}

    RSA * getRsa();
    std::string getAsn1PublicKey();
    std::string getVerifyToken();
    std::string getServerId();
    std::string getPublicKey();
    
    static MinecraftServer & getServer();
protected:
    MinecraftServerData *m; // PIMPL
};

} /* namespace MCServer */
#endif /* MINECRAFTSERVER_HPP_ */
