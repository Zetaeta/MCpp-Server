
#include <map>

#include "EntityManager.hpp"
#include "entity/Entity.hpp"
#include "MinecraftServer.hpp"

namespace MCServer {

using std::map;
using Entities::Entity;

struct EntityManagerData {
    map<int, Entity *> entities;
    int lastUsedId;

    MinecraftServer *server;

    EntityManagerData()
    :lastUsedId(0) {}
};

EntityManager::EntityManager(MinecraftServer *server)
:m(new EntityManagerData) {
    m->server = server;
}

int EntityManager::getNewEntityId() {
    return m->lastUsedId++;
}


}
