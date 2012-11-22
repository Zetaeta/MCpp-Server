
#include <string>

#include "PlayerChatEvent.hpp"

namespace MCServer {
namespace Events {

using std::string;
using std::shared_ptr;

using Entities::Player;

PlayerChatEvent::PlayerChatEvent(const string &message, const shared_ptr<Player> &sender)
:PlayerEvent(EventType::PLAYER_CHAT, sender), message(message) {
}

string PlayerChatEvent::getMessage() {
    return message;
}

void PlayerChatEvent::setMessage(const std::string &msg) {
    message = msg;
}

}
}
