
#include <string>

#include "PlayerDigEvent.hpp"

namespace MCServer {
namespace Events {

using std::string;
using std::shared_ptr;

using Entities::Player;

PlayerDigEvent::PlayerDigEvent(const shared_ptr<Player> &sender, Block &block)
:PlayerEvent(EventType::PLAYER_CHAT, sender), message(message) {
}

string PlayerDigEvent::getMessage() {
    return message;
}

void PlayerDigEvent::setMessage(const std::string &msg) {
    message = msg;
}

}
}
