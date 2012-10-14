
#include <ctime>
#include <iostream>

#include "UIHandler.hpp"
#include "MinecraftServer.hpp"
#include "ui/UIManager.hpp"

namespace MCServer {
namespace Logging {

struct UIHandlerData {
    MinecraftServer *server;
};

UIHandler::UIHandler(MinecraftServer *server)
 :m(new UIHandlerData()) {
    m->server = server;
}

void UIHandler::handle(Level level, const std::string &message) {
    
    m->server->getUIManager().println(timeString() + " [" + levelName(level) + "] " + message);
}

Handler * UIHandler::clone() {
    return new UIHandler(*this);
}

std::string UIHandler::timeString() {
#define TIMELENGTH 10
    char timeStr[TIMELENGTH];
    time_t currTime;
    time(&currTime);
    struct tm *localTime = localtime(&currTime);
    strftime(timeStr, TIMELENGTH, "%H:%M:%S", localTime);
    timeStr[TIMELENGTH - 1] = '\0';
    return timeStr;
}

std::string UIHandler::levelName(Level level) {
    switch (level) {
    case FINEST:
        return "FINEST";
    case FINE:
        return "FINE";
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case SEVERE:
        return "SEVERE";
    default:
        return "NULL";
    }
}

}
}
