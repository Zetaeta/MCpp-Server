
#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

namespace MCServer {

struct EntityManagerData;

class EntityManager {
public:
    int getNewEntityId();
private:
    EntityManagerData *m;
};

}

#endif
