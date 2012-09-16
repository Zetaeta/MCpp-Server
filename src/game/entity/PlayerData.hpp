
#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

#include <string>

#include "EntityData.hpp"

namespace MCServer {
namespace Entities {

struct PlayerData : public EntityData {
    std::string name;
};

}
}

#endif
