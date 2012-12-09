
#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

#include <string>

#include "EntityData.hpp"
#include "game/PlayerInventory.hpp"
#include "game/Block.hpp"
#include "game/GameMode.hpp"

namespace MCServer {

enum GameMode : uint8_t;

namespace Network {
class ClientConnection;
}

namespace Entities {

struct PlayerData : public EntityData {
    Network::ClientConnection *connection;

    std::string name;

    bool onGround;
    bool sleeping;
    short air;
    short attackTime;
    short deathTime;
    short fire;
    short health;
    short hurtTime;
    short sleepTimer;
    int foodLevel;
    int foodTickTimer;
    GameMode playerGameType;
    int xpLevel;
    int xpTotal;
    float fallDistance;
    float foodExhaustionLevel;
    float foodSaturationLevel;
    float xpP;

    PlayerInventory inventory;

    unsigned long diggingTime; // Time digging was started.
    Block diggingTarget;

    PlayerData()
    :onGround(true), sleeping(false), air(0), attackTime(0), deathTime(0), fire(0), health(0), hurtTime(0), sleepTimer(0),
    foodLevel(0), foodTickTimer(0), playerGameType(SURVIVAL), xpLevel(0), xpTotal(0), fallDistance(0),
    foodExhaustionLevel(0), foodSaturationLevel(0), xpP(0), diggingTime(0) {}
};

}
}

#endif
