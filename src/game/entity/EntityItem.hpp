
#ifndef ENTITYITEM_HPP
#define ENTITYITEM_HPP

#include <stdint.h>

#include "Entity.hpp"

namespace MCServer {

struct Point3D;

namespace Entities {

struct EntityItemData;

class EntityItem : public Entity {
public:
    EntityItem(uint16_t id, uint8_t count, Point3D location);

    uint16_t getId() const;
    uint8_t getCount() const;

    void setId(uint16_t);
    void setCount(uint8_t);
private:
    uint16_t itemId;
    uint8_t count;
};

}
}

#endif
