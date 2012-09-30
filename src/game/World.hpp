
#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>

#include "int128.h"
#include "MyExcept.hpp"

namespace MCServer {

class Chunk;
class Point2D;
struct WorldData;
class ChunkCoordinates;

class World {
public:
    World();
    World(const std::string &name);

    void loadFrom(const std::string &directory) throws(WorldLoadingFailure);
    void readRegionFile(const std::string &fileName);

    Chunk & loadChunk(const ChunkCoordinates &);
    void unloadChunk(const ChunkCoordinates &);

    const std::string & getName();
    Chunk & chunkAt(int x, int y);
    Chunk & chunkAt(const Point2D &pt);
    Chunk & chunkAt(const ChunkCoordinates &);
private:
    WorldData *m;
};

}

#endif

