
#include "Logger.hpp"
#include "Handler.hpp"
#include <vector>
#include <map>

namespace MCServer {
namespace Logging {

using std::vector;
using std::string;

struct LoggerData {
    Level defaultLevel;
    vector<Handler> handlers;

    static map<string, Logger *> loggers;
}

Logger::Logger(std::string name)
 :m(new LoggerData()) {

}


void Logger::log(Level level, string message) {
    for (auto it = handlers.begin(); it != handlers.end(); ++it) {
        (*it)->handle(level, message);
    }
}

void Logger::addHandler(Handler handler) {
    handlers.add(handler);
}

Logger & Logger::getLogger(string name) {
    Logger log = new Logger(name);
    LoggerData::loggers[name] = log;
    return *log;
}

void Logger::destroyLogger(Logger &logger) {
    LoggerData::loggers.erase(&logger);
    delete &logger;
}

}
}

