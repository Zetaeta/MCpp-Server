
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Util/stlfwd.hpp>

#include "Entity.hpp"

namespace MCServer {

class World;

namespace Network {
class ClientConnection;
}

namespace Entities {

class Player : public Entity {
public:
    Player(std::string name, Network::ClientConnection &);
    ~Player();

    void loadData();
    World & getWorld() const;
    std::string getName();
    void sendKeepAlive();
    void sendMessage(const std::string &);
    Network::ClientConnection & getConnection();

    friend class MCServer::Network::ClientConnection;
};

}
}

#endif
