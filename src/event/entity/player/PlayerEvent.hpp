
#ifndef PLAYEREVENT_HPP
#define PLAYEREVENT_HPP

#include <memory>

#include "event/Event.hpp"

namespace MCServer {

namespace Entities {
class Player;
}

namespace Events {

class PlayerEvent : public Event {
public:
    const std::shared_ptr<Entities::Player> & getPlayer();
protected:
    PlayerEvent(EventType::EventType e, const std::shared_ptr<Entities::Player> &p) :Event(e), player(p) {}
    std::shared_ptr<Entities::Player> player;
};

}
}

#endif
