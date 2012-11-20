
#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace MCServer {

struct Point3D;

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
protected:
    Entity(EntityData *, int id);
    Entity(EntityData *);
    EntityData *m;
};

}
}

#endif
