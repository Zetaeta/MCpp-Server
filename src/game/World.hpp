
#ifndef WORLD_HPP
#define WORLD_HPP #include <memory>

#include <Util/stlfwd.hpp>
#include "int128.h"
#include "MyExcept.hpp"

namespace MCServer {

class Chunk;
class Point2D;
struct WorldData;
class ChunkCoordinates;

namespace Entities {
struct PlayerData;
}

class World {
public:
    World();
    World(const std::string &name);
    World(const World &);
    World(World &&);

    World & operator=(World &&);

    void loadFrom(const std::string &directory) throws(WorldLoadingFailure);
    void readRegionFile(const std::string &fileName);

    Chunk & loadChunk(const ChunkCoordinates &);
    void unloadChunk(const ChunkCoordinates &);
    std::vector<Chunk *> loadAll(const std::vector<ChunkCoordinates> &coords);
    void loadPlayer(Entities::PlayerData *);

    std::string getName() const;
    int getDimension() const;

    Chunk & chunkAt(int x, int y);
    Chunk & chunkAt(const Point2D &pt);
    Chunk & chunkAt(const ChunkCoordinates &);
private:

    WorldData *m;
};

}

#endif

