
#include <iostream>

#include "Entity.hpp"
#include "EntityData.hpp"
#include "MinecraftServer.hpp"
#include "game/EntityManager.hpp"
#include "game/World.hpp"

namespace MCServer {
namespace Entities {

using std::shared_ptr;
using std::weak_ptr;

Entity::Entity(EntityData *m, int id)
:m(m) {
    m->id = id;
}
Entity::Entity(EntityData *m)
:m(m) {
    m->id = MinecraftServer::getServer().getEntityManager().getNewEntityId();
}

Entity::Entity(EntityData *m, Point3D location, int id)
:m(m) {
    m->id = id;
    m->position = location;
}
Entity::Entity(EntityData *m, Point3D location)
:m(m) {
    m->id = MinecraftServer::getServer().getEntityManager().getNewEntityId();
    m->position = location;
}

Point3D Entity::getPosition() const {
    return m->position;
}

void Entity::setPosition(const Point3D &pos) {
    auto orig = m->position;
    m->position = pos;
    getWorld().entityMoved(*this, orig, pos);
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

World & Entity::getWorld() const {
//    return MinecraftServer::server().getWorld(m->dimension);
    return MinecraftServer::server().getMainWorld(); // TODO: Get world properly.
}

Entity::~Entity() {
    delete m;
}


int Entity::getId() {
    return m->id;
}

int Entity::getEntityId() {
    return m->id;
}

void Entity::setReference(const shared_ptr<Entity> &ref) {
    m->reference = ref;
}

shared_ptr<Entity> Entity::getReference() {
    return m->reference.lock();
}

}
}
