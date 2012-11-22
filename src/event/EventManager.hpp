
#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <map>
#include <vector>

#include <Util/stlfwd.hpp>

#include "EventType.hpp"

namespace MCServer {
namespace Events {

class EventHandler;
class Event;

class EventManager {
    typedef std::function<void (Event &event)> EventHandlerFunc;
public:
    void registerListener(EventType::EventType, const EventHandlerFunc &);
    void registerListener(EventType::EventType, EventHandler &);
    void dispatchEvent(Event &);
private:
    std::map<EventType::EventType, std::vector<EventHandlerFunc>> listeners;
};

}
}

#endif
