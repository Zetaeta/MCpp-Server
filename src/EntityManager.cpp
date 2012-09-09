
#include <map>

#include "EntityManager.hpp"
#include "entity/Entity.hpp"

namespace MCServer {

using std::map;
using Entities::Entity;

struct EntityManagerData {
    map<int, Entity *> entities;
    int lastUsedId;
    EntityManagerData()
    :lastUsedId(0) {}
};

int EntityManager::getNewEntityId() {
    return m->lastUsedId++;
}


}
