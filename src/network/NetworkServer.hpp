/*
 * NetworkServer.hpp
 *
 *  Created on: 4 Aug 2012
 *      Author: daniel
 */

#ifndef NETWORKSERVER_HPP_
#define NETWORKSERVER_HPP_

struct sockaddr_in;

#include "../MinecraftServer.hpp"

namespace MCServer {
namespace Network {

struct NetworkServerData;
struct ClientConnection;

class NetworkServer {
public:
    NetworkServer(MinecraftServer *);
    virtual ~NetworkServer();
    void run();
//    void acceptClient(boost::asio::ip::tcp::socket *socket);
protected:
    void init();
    void handleAccept(const ClientConnection &);
    void serverListPing(const ClientConnection &);

    NetworkServerData *m;
};

} /* namespace Network */
} /* namespace MCServer */
#endif /* NETWORKSERVER_HPP_ */
