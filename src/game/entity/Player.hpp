
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "Entity.hpp"

namespace MCServer {
namespace Entities {

class Player : public Entity {
public:
    Player(std::string name);

};

}
}

#endif
