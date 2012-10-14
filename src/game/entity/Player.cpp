
#include "Player.hpp"
#include "PlayerData.hpp"
#include "Pimpl.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"
#include "game/World.hpp"

namespace MCServer {
namespace Entities {

using std::string;
using Logging::Logger;

Player::Player(string name)
:Entity(new PlayerData) {
    D(Player);
    m->name = name;
}

void Player::loadData() {
    World &defaultWorld = MinecraftServer::server().getWorld(0);
    defaultWorld.loadPlayer(static_cast<PlayerData *>(m));
}

World & Player::getWorld() const {
    D(Player);
    return MinecraftServer::server().getWorld(m->dimension);
}

}
}

