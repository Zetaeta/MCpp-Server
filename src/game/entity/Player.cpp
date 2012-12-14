
#include <vector>
#include <sstream>

#include "Player.hpp"
#include "PlayerData.hpp"
#include "Pimpl.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"
#include "game/World.hpp"
#include "network/ClientConnection.hpp"
#include "util/Utils.hpp"
#include "game/Chunk.hpp"
#include "game/BlockType.hpp"

namespace MCServer {
namespace Entities {

using std::string;
using Logging::Logger;
using Network::ClientConnection;
using std::shared_ptr;

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
    D(Player);
    shared_ptr<Chunk> chunk = getWorld().chunkAt(Point2D(x, z));
    std::cout << "Player::setDigging(): x = " << x << ", y = " << uint16_t(y) << ", z = " << z << ", chunk = " << chunk.get() << '\n';
    m->diggingTarget = (*chunk)[x % 16][z % 16][y];
    m->diggingTime = currentTimeMillis();
}

void Player::finishDigging() {
    D(Player);
    long diggingLength = currentTimeMillis() - m->diggingTime;
    std::cout << "m->diggingTime = " << m->diggingTime << ", currentTimeMillis() = " << currentTimeMillis() << '\n';
    Block &block = m->diggingTarget;
    if (!block) {
        // TODO: Do something
        return;
    }
    float hardness = blockTypes[block.getId()].hardness;
    float shouldHaveTaken = 1000 * hardness * 1.5;
    if (false) { // TODO: If current tool cannot harvest it
        shouldHaveTaken *= 3.33;
    }
    if (diggingLength < (shouldHaveTaken - 100 )) { // 100 overhead to account for lag 'n' stuff.
        sendMessage("You failed to break the block!");
        std::ostringstream oss;
        oss << "Required time: " << shouldHaveTaken << ", you took: " << diggingLength;
        sendMessage(oss.str());
        return;
    }
    // TODO: Penalties for in air, water etc.
    // TODO: Drop item
    getWorld().dropItem(ItemStack(block.getId(), 1), block.getPosition());
    sendMessage("You broke the block!");
    std::ostringstream oss;
    oss << "Required time: " << shouldHaveTaken << ", you took: " << diggingLength;
    sendMessage(oss.str());
}

void Player::cancelDigging() {
    D(Player);
    m->diggingTime = 0;
}

PlayerInventory & Player::getInventory() {
    D(Player);
    return m->inventory;
}

const PlayerInventory & Player::getInventory() const {
    D(Player);
    return m->inventory;
}

}
}

