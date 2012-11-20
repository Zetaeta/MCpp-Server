
#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <Util/stlfwd.hpp>
#include "int128.h"
#include "MyExcept.hpp"

namespace MCServer {

class Chunk;
struct Point2D;
struct WorldData;
struct ChunkCoordinates;
class ReentrantLock;

namespace Entities {
class Entity;
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

    std::shared_ptr<Chunk> createChunk(const ChunkCoordinates &pos);

    std::shared_ptr<Chunk> loadChunk(const ChunkCoordinates &);
    void unloadChunk(const ChunkCoordinates &);
    void unloadChunk(const std::shared_ptr<Chunk> &);

    void saveChunk(const ChunkCoordinates &);
    void saveChunk(const std::shared_ptr<Chunk> &);

    std::vector<std::shared_ptr<Chunk>> loadAll(const std::vector<ChunkCoordinates> &coords);
    std::vector<std::shared_ptr<Chunk>> getLoadedChunks() const;
    std::map<ChunkCoordinates, std::weak_ptr<Chunk>> & getUnloadingChunks();
    const std::map<ChunkCoordinates, std::weak_ptr<Chunk>> & getUnloadingChunks() const;
    ReentrantLock & getChunksLock();
    ReentrantLock & getUnloadingChunksLock();

    void loadPlayer(Entities::PlayerData *);

    std::string getName() const;
    int getDimension() const;

    std::shared_ptr<Chunk> chunkAt(int x, int y) const;
    std::shared_ptr<Chunk> chunkAt(const Point2D &pt) const;
    std::shared_ptr<Chunk> chunkAt(const ChunkCoordinates &) const;

    void addEntity(const std::shared_ptr<Entities::Entity> &);
    void removeEntity(const std::shared_ptr<Entities::Entity> &);
    std::vector<std::shared_ptr<Entities::Entity>> & getEntities();
    const std::vector<std::shared_ptr<Entities::Entity>> & getEntities() const;
private:

    WorldData *m;
};

}

#endif

