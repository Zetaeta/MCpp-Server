
#ifndef ENTITYITEMDATA_HPP
#define ENTITYITEMDATA_HPP

#include "EntityData.hpp"

namespace MCServer {
namespace Entities {

struct EntityItemData : public EntityData {
    uint16_t id;
    uint8_t count;
};

}
}

#endif
