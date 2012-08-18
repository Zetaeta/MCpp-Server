
#include <ctime>

#include "ConsoleHandler.hpp"

namespace MCServer {
namespace Logging {

void ConsoleHandler::handle(Level level, std::string message) {
    
    std::cout << timeString() << " [" << levelName(level) << "] " << message << '\n';
}

std::string ConsoleHandler::timeString() {
#define TIMELENGTH 10
    char timeStr[TIMELENGTH];
    time_t currTime;
    time(&currTime);
    struct tm *localTime = localtime(&currTime);
    strftime(timeStr, TIMELENGTH, "%H:%M:%S", localTime);
    return buf;
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
    }
}

}
}
