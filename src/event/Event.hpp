
#ifndef EVENT_HPP
#define EVENT_HPP

#include "EventType.hpp"

namespace MCServer {

namespace Events {

class Event {
public:
    EventType::EventType getType() const {
        return type;
    }

    virtual ~Event() = default;
protected:
    Event(EventType::EventType);
private:
    EventType::EventType type;
};

}

}

#endif
