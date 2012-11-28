
#ifndef ENTITYDATA_HPP
#define ENTITYDATA_HPP

#include <memory>

#include "game/Point3D.hpp"

namespace MCServer {
namespace Entities {

struct EntityData {
    int id;
    Point3D position;
    double pitch, yaw, stance;
    int dimension;

    std::weak_ptr<Entity> reference;
};

}
}

#endif
