
#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>

#include "int128.h"

namespace MCServer {

struct WorldData;

class World {
public:
    World(const std::string &name, uint128_t uid);

    Chunk & chunkAt(int x, int y);
    Chunk & chunkAt(const Point &pt);
private:
    WorldData *m;
};

}

#endif

