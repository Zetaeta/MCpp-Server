
#include <algorithm>
#include <memory>
#include <unistd.h>

#include "ChunkLoader.hpp"
#include "Scheduler.hpp"
#include "ReentrantLock.hpp"
#include "AutoLock.hpp"
#include "MinecraftServer.hpp"
#include "ChunkCoordinates.hpp"
#include "Chunk.hpp"
#include "Point3D.hpp"
#include "World.hpp"
#include "entity/Player.hpp"

using std::dynamic_pointer_cast;
using std::shared_ptr;


namespace MCServer {

using Entities::Player;
using Entities::Entity;

void ChunkLoader::start() {
    MinecraftServer::server().getScheduler().startImportantThread("Chunk Loader" ,&ChunkLoader::run, this);
}

void ChunkLoader::run() {
    while (MinecraftServer::server().isRunning()) {
        auto worlds = MinecraftServer::server().getWorlds();
        for (World *world : worlds) {
            auto loadedChunks = world->getLoadedChunks();
            auto al = autoLock(world->getChunksLock());
            for(shared_ptr<Chunk> chunk : loadedChunks) {
                bool unloadable = true;
                ChunkCoordinates coords = chunk->getCoordinates();
                auto &entities = world->getEntities();
                for (shared_ptr<Entity> &entity : entities) {
                    shared_ptr<Player> player;
                    if ((player = dynamic_pointer_cast<Player>(entity)) == nullptr) {
                        return;
                    }
                    ChunkCoordinates playerCoords = player->getPosition();
                    if (((coords.x - playerCoords.x) < 3 || (playerCoords.x - coords.x) < 3)
                   && ((coords.z - playerCoords.z) < 3 || (playerCoords.z - coords.z) < 3)) {
                        unloadable = false;
                    }
                }
                if (unloadable) {
                    world->unloadChunk(coords);
                }
            }
        }
        for (World *world : worlds) {
            auto chunksToUnload = world->getUnloadingChunks();
            auto &unloadingChunksLock = world->getUnloadingChunksLock();
            AUTOLOCK(unloadingChunksLock);
            for (auto it = chunksToUnload.begin(); it != chunksToUnload.end(); ++it) {
                if (it->second.expired()) {
                    it = chunksToUnload.erase(it);
                }
            }
        }
        sleep(1);
    }
}

}

