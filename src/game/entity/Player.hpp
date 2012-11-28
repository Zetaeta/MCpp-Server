
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <stdint.h>

#include <Util/stlfwd.hpp>

#include "Entity.hpp"
#include "CommandSender.hpp"

namespace MCServer {

class World;

namespace Network {
class ClientConnection;
}

namespace Entities {

class Player : public Entity, public CommandSender {
public:
    Player(std::string name, Network::ClientConnection &);
    ~Player();

    void loadData();
    std::string getName() const;
    void sendKeepAlive();
    void sendMessage(const std::string &);
    Network::ClientConnection & getConnection();

    void setDigging(int x, uint8_t y, int z, uint8_t face);
    void setDigging(bool);

    friend /* with benefits */ class MCServer::Network::ClientConnection;
};

}
}

#endif
