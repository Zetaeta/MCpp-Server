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
//    boost::asio::io_service & getIoService();
protected:
    MinecraftServerData *m; // PIMPL
};

} /* namespace MCServer */
#endif /* MINECRAFTSERVER_HPP_ */
