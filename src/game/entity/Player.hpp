
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "Entity.hpp"

namespace MCServer {

class World;

namespace Network {
class ClientConnection;
}

namespace Entities {

class Player : public Entity {
public:
    Player(std::string name);
    void loadData();
    World & getWorld() const;

    friend class MCServer::Network::ClientConnection;
};

}
}

#endif
