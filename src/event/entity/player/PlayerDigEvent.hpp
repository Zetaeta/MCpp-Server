
#ifndef PLAYERDIGEVENT_HPP
#define PLAYERDIGEVENT_HPP

#include <Util/stlfwd.hpp>

#include "event/Cancellable.hpp"
#include "PlayerEvent.hpp"

namespace MCServer {

class CommandSender;

namespace Entities {
class Player;
}

namespace Events {

class PlayerDigEvent : public PlayerEvent, public Cancellable {
public:
    PlayerDigEvent(const std::string &, const std::shared_ptr<Entities::Player> &);
    std::string getMessage();
    void setMessage(const std::string &);
private:
    std::string message;
};

}
}

#endif
