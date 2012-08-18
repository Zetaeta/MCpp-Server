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

namespace MCServer {

namespace Logging {
class Logger;
}

namespace Network {
class NetworkServer;
}

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
protected:
    MinecraftServerData *m; // PIMPL
};

} /* namespace MCServer */
#endif /* MINECRAFTSERVER_HPP_ */
