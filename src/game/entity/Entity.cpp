
#include "Entity.hpp"
#include "EntityData.hpp"
#include "MinecraftServer.hpp"
#include "EntityManager.hpp"

namespace MCServer {
namespace Entities {


Entity::Entity(EntityData *m, int id)
:m(m) {
    m->id = id;
}
Entity::Entity(EntityData *m)
:m(m) {
    m->id = MinecraftServer::getServer().getEntityManager().getNewEntityId();
}

Entity::~Entity() {
    delete m;
}


int Entity::getId() {
    return m->id;
}

}
}
