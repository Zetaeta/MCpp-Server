/*
 * NetworkServer.hpp
 *
 *  Created on: 4 Aug 2012
 *      Author: daniel
 */

#ifndef NETWORKSERVER_HPP_
#define NETWORKSERVER_HPP_

#include "../MinecraftServer.hpp"

namespace MCServer {
namespace Network {

struct NetworkServerData;

class NetworkServer {
public:
    NetworkServer(MinecraftServer *);
    virtual ~NetworkServer();
    void run();
//    void acceptClient(boost::asio::ip::tcp::socket *socket);
protected:
    void init();

    NetworkServerData *m;
};

} /* namespace Network */
} /* namespace MCServer */
#endif /* NETWORKSERVER_HPP_ */
