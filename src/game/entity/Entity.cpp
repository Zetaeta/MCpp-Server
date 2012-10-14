
#include "Entity.hpp"
#include "EntityData.hpp"
#include "MinecraftServer.hpp"
#include "game/EntityManager.hpp"

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

Point3D Entity::getPosition() const {
    return m->position;
}

Entity::~Entity() {
    delete m;
}


int Entity::getId() {
    return m->id;
}

}
}
