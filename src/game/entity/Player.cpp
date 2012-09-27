
#include "Player.hpp"
#include "PlayerData.hpp"
#include "Pimpl.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"

namespace MCServer {
namespace Entities {

using std::string;
using Logging::Logger;

Player::Player(string name)
:Entity(new PlayerData) {
    D(Player);
//    Logger &log = MinecraftServer::getServer().getLogger();
//    log << "this->m: " << reinterpret_cast<long>(this->m) << ", m: " << reinterpret_cast<long>(m) << '\n';
//    log << "Entity id: " << this->m->id << '\n';
//    log << "Current name (should be blank): \"" << m->name << "\".\n";
//    log << "Assigning name: \"" << name << "\".\n";
    m->name = name;
}

}
}

