
#ifndef ENTITYDATA_HPP
#define ENTITYDATA_HPP

#include "game/Point3D.hpp"

namespace MCServer {
namespace Entities {

struct EntityData {
    int id;
    Point3D position;
    double pitch, yaw, stance;
};

}
}

#endif
