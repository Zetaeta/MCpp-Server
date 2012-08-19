
#include <ctime>
#include <iostream>

#include "ConsoleHandler.hpp"
#include "../MinecraftServer.hpp"
#include "../ConsoleReader.hpp"

namespace MCServer {
namespace Logging {

struct ConsoleHandlerData {
    MinecraftServer *server;
};

ConsoleHandler::ConsoleHandler(MinecraftServer *server)
 :m(new ConsoleHandlerData()) {
    m->server = server;
}

void ConsoleHandler::handle(Level level, std::string message) {
    
    m->server->getConsoleReader().println(timeString() + " [" + levelName(level) + "] " + message);
}

Handler * ConsoleHandler::clone() {
    return new ConsoleHandler(*this);
}

std::string ConsoleHandler::timeString() {
#define TIMELENGTH 10
    char timeStr[TIMELENGTH];
    time_t currTime;
    time(&currTime);
    struct tm *localTime = localtime(&currTime);
    strftime(timeStr, TIMELENGTH, "%H:%M:%S", localTime);
    return timeStr;
}

std::string ConsoleHandler::levelName(Level level) {
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
