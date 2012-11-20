
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

void Entity::setPosition(const Point3D &pos) {
    m->position = pos;
}

double Entity::getYaw() {
    return m->yaw;
}

void Entity::setYaw(double yaw) {
    m->yaw = yaw;
}

double Entity::getPitch() {
    return m->pitch;
}

void Entity::setPitch(double pitch) {
    m->pitch = pitch;
}

double Entity::getStance() {
    return m->stance;
}

void Entity::setStance(double stance) {
    m->stance = stance;
}

Entity::~Entity() {
    delete m;
}


int Entity::getId() {
    return m->id;
}

}
}
