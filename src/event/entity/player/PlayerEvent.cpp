
#include "PlayerEvent.hpp"

namespace MCServer {
namespace Events {

using std::shared_ptr;

const shared_ptr<Entities::Player> & PlayerEvent::getPlayer() {
    return player;
}

}
}
