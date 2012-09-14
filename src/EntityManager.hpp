
#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

namespace MCServer {

class MinecraftServer;

struct EntityManagerData;

class EntityManager {
public:
    EntityManager(MinecraftServer *server);

    int getNewEntityId();
private:
    EntityManagerData *m;
};

}

#endif
