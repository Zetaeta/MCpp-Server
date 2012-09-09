
#include "Entity.hpp"

namespace MCServer {
namespace Entities {

struct EntityData {
    int id;
};

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

}
}
