
#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace MCServer {
namespace Entities {

struct EntityData;

class Entity {
public:
    virtual ~Entity();

    int getId();
protected:
    Entity(EntityData *, int id);
    Entity(EntityData *);
    EntityData *m;
};

}
}

#endif
