
#include <vector>

#include "Player.hpp"
#include "PlayerData.hpp"
#include "Pimpl.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"
#include "game/World.hpp"
#include "network/ClientConnection.hpp"

namespace MCServer {
namespace Entities {

using std::string;
using Logging::Logger;
using Network::ClientConnection;

Player::Player(string name, Network::ClientConnection &cc)
:Entity(new PlayerData) {
    D(Player);
    m->name = name;
    m->connection = &cc;
}

void Player::loadData() {
    World &defaultWorld = MinecraftServer::server().getWorld(0);
    defaultWorld.loadPlayer(static_cast<PlayerData *>(m));
}

string Player::getName() const {
    D(Player);
    return m->name;
}

Player::~Player() {
    D(Player);
    delete m->connection;
}

void Player::sendKeepAlive() {
    D(Player);
    m->connection->sendKeepAlive();
}

void Player::sendMessage(const string &message) {
    D(Player);
    m->connection->sendMessage(message);
}

ClientConnection & Player::getConnection() {
    D(Player);
    return *m->connection;
}

void Player::setDigging(int x, uint8_t y, int z, uint8_t face) {

}

void Player::setDigging(bool) {

}

}
}

