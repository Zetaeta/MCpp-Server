
#include <functional>

#include "EventManager.hpp"
#include "EventHandler.hpp"
#include "Event.hpp"

namespace MCServer {
namespace Events {

void EventManager::registerListener(EventType::EventType type, const EventHandlerFunc &func) {
    listeners[type].push_back(func);
}

void EventManager::registerListener(EventType::EventType type, EventHandler &handler) {
    listeners[type].push_back([&] (Event &event) { handler.handleEvent(&event); });
}

void EventManager::dispatchEvent(Event &event) {
    auto it = listeners.find(event.getType());
    if (it != listeners.end()) {
        for (const EventHandlerFunc &func : it->second) {
            func(event);
        }
    }
}

}
}
