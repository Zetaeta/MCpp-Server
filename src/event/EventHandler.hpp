
#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

namespace MCServer {
namespace Events {

class Event;

class EventHandler {
public:
    virtual void handleEvent(Event *event);
};

}
}

#endif
