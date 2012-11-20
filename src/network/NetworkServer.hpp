
#ifndef NETWORKSERVER_HPP_
#define NETWORKSERVER_HPP_

struct sockaddr_in;

#include "MinecraftServer.hpp"

namespace MCServer {
namespace Network {

struct NetworkServerData;
struct Connection;

class NetworkServer {
public:
    NetworkServer(MinecraftServer *);
    ~NetworkServer();
    void run();
    void shutdown();
protected:
    void init();
    void handleAccept(const Connection &);
    void serverListPing(const Connection &);

    NetworkServerData *m;
};

} /* namespace Network */
} /* namespace MCServer */
#endif /* NETWORKSERVER_HPP_ */
