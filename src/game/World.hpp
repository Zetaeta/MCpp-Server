
#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>

#include <Util/stlfwd.hpp>
#include "int128.h"
#include "MyExcept.hpp"

namespace MCServer {

class Chunk;
struct Point2D;
struct Point3D;
struct WorldData;
struct ChunkCoordinates;
class ReentrantLock;
class ItemStack;

namespace Entities {
class Entity;
struct PlayerData;
class EntityItem;
}

using Entities::Entity;

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

    void saveAll();

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

    void addEntity(const std::shared_ptr<Entity> &);
    void removeEntity(const std::shared_ptr<Entity> &);
    std::vector<std::shared_ptr<Entity>> & getEntities();
    const std::vector<std::shared_ptr<Entity>> & getEntities() const;

    std::shared_ptr<Entities::EntityItem> dropItem(const ItemStack &, Point3D);

    void entityMoved(Entity &, Point3D from, Point3D to);

    static int chunkToRegion(int chunkCoord);
    static size_t headerOffset(int x, int z);
private:

    WorldData *m;
};

}

#endif

