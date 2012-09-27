
#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>

#include "int128.h"
#include "MyExcept.hpp"

namespace MCServer {

class Chunk;
class Point2D;
struct WorldData;

class World {
public:
    World();
    World(const std::string &name, uint128_t uid);

    void loadFrom(const std::string &directory) throws(WorldLoadingFailure);
    void readRegionFile(const std::string &fileName);

    const std::string & getName();
    Chunk & chunkAt(int x, int y);
    Chunk & chunkAt(const Point2D &pt);
private:
    WorldData *m;
};

}

#endif

