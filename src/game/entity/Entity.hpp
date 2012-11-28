
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>

namespace MCServer {

struct Point3D;
class World;

namespace Entities {

struct EntityData;

class Entity {
public:
    virtual ~Entity();

    int getId();
    Point3D getPosition() const;
    void setPosition(const Point3D &);
    double getYaw();
    double getPitch();
    void setYaw(double);
    void setPitch(double);
    double getStance();
    void setStance(double);
    World & getWorld() const;

    void setReference(const std::shared_ptr<Entity> &);
    std::shared_ptr<Entity> getReference();
protected:
    Entity(EntityData *, int id);
    Entity(EntityData *);
    EntityData *m;
};

}
}

#endif
