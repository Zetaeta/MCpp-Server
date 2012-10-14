
#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

#include <string>

#include "EntityData.hpp"

namespace MCServer {

enum GameMode : uint8_t;

namespace Entities {

struct PlayerData : public EntityData {
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
    int dimension;
    int foodLevel;
    int foodTickTimer;
    GameMode playerGameType;
    int xpLevel;
    int xpTotal;
    float fallDistance;
    float foodExhaustionLevel;
    float foodSaturationLevel;
    float xpP;
    Point3D position;
};

}
}

#endif
