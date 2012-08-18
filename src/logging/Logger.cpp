
#include <vector>
#include <map>

#include "Logger.hpp"
#include "Handler.hpp"
#include "ConsoleHandler.hpp"

namespace MCServer {
namespace Logging {

using std::vector;
using std::string;
using std::map;

struct LoggerData {
    string name;
    Level defaultLevel;
    vector<Handler *> handlers;

    static map<string, Logger *> loggers;
};

map<string, Logger *> LoggerData::loggers;

Logger::Logger(std::string name)
 :m(new LoggerData()) {
    m->name = name;
    m->defaultLevel = INFO;
}


void Logger::log(Level level, string message) {
    for (auto it =m->handlers.begin(); it != m->handlers.end(); ++it) {
        (*it)->handle(level, message);
    }
}

void Logger::log(string message) {
    log(m->defaultLevel, message);
}

void Logger::addHandler(Handler *handler) {
    m->handlers.push_back(handler->clone());
}

Logger & Logger::getLogger(string name) {
    Logger *log = new Logger(name);
    LoggerData::loggers[name] = log;
    log->addHandler(new ConsoleHandler());
    return *log;
}

void Logger::destroyLogger(Logger &logger) {
    LoggerData::loggers.erase(logger.m->name);
    delete &logger;
}

void Logger::setDefaultLevel(Level level) {
    m->defaultLevel = level;
}

Logger::~Logger() {
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
        delete *it;
    }
}

}
}

