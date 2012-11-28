
#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <stdint.h>

namespace MCServer {

class MinecraftServer;

struct EntityManagerData;

class EntityManager {
public:
    EntityManager(MinecraftServer *server);

    void sendAnimation(int entityId, uint8_t animation);

    int getNewEntityId();
private:
    EntityManagerData *m;
};

}

#endif
