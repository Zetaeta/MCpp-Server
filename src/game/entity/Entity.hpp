
#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace MCServer {

class Point3D;

namespace Entities {

struct EntityData;

class Entity {
public:
    virtual ~Entity();

    int getId();
    Point3D getPosition() const;
protected:
    Entity(EntityData *, int id);
    Entity(EntityData *);
    EntityData *m;
};

}
}

#endif
