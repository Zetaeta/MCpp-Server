
#ifndef PLAYERWORLDDATA_HPP
#define PLAYERWORLDDATA_HPP

#include "Point3D.hpp"

namespace MCServer {

enum GameMode : uint8_t;

struct PlayerWorldData {
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

#endif
