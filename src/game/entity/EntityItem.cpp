
#include <iostream>

#include "EntityItem.hpp"
#include "EntityItemData.hpp"
#include "game/Point3D.hpp"
#include "Pimpl.hpp"

namespace MCServer {
namespace Entities {

EntityItem::EntityItem(uint16_t id, uint8_t count, Point3D location)
:Entity(new EntityItemData, location) {
    D(EntityItem);
    m->id = id;
    m->count = count;
    std::cout << "EntityItem(): location = " << location << ", position = " << getPosition() << '\n';
}

uint16_t EntityItem::getId() const {
    D(EntityItem);
    return m->id;
}

uint8_t EntityItem::getCount() const {
    D(EntityItem);
    return m->count;
}

void EntityItem::setId(uint16_t id) {
    D(EntityItem);
    m->id = id;
}

void EntityItem::setCount(uint8_t count) {
    D(EntityItem);
    m->count = count;
}

}
}
