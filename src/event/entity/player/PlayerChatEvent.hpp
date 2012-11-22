
#ifndef PLAYERCHATEVENT_HPP
#define PLAYERCHATEVENT_HPP

#include <Util/stlfwd.hpp>

#include "event/Cancellable.hpp"
#include "PlayerEvent.hpp"

namespace MCServer {

class CommandSender;

namespace Entities {
class Player;
}

namespace Events {

class PlayerChatEvent : public PlayerEvent, public Cancellable {
public:
    PlayerChatEvent(const std::string &, const std::shared_ptr<Entities::Player> &);
    std::string getMessage();
    void setMessage(const std::string &);
private:
    std::string message;
};

}
}

#endif
