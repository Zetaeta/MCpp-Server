
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <bitset>

#include "Logger.hpp"
#include "Handler.hpp"
#include "ConsoleHandler.hpp"
#include "../MinecraftServer.hpp"

namespace MCServer {
namespace Logging {

using std::vector;
using std::string;
using std::map;
using std::ostringstream;

struct LoggerData {
    string name;
    Level defaultLevel;
    vector<Handler *> handlers;
    ostringstream oss;

    static map<string, Logger *> loggers;
};

map<string, Logger *> LoggerData::loggers;

Logger::Logger(std::string name)
 :m(new LoggerData()) {
    m->name = name;
    m->defaultLevel = INFO;
}


void Logger::log(Level level, string message) {
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
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
    Logger *log;
    if ((log = LoggerData::loggers[name]) != 0) {
        return *log;
    }
    log = new Logger(name);
    LoggerData::loggers[name] = log;
    log->addHandler(new ConsoleHandler(&MinecraftServer::getServer()));
    return *log;
}

void Logger::destroyLogger(Logger &logger) {
    LoggerData::loggers.erase(logger.m->name);
    delete &logger;
}

void Logger::setDefaultLevel(Level level) {
    m->defaultLevel = level;
}

Logger & Logger::operator<<(Level level) {
    m->defaultLevel = level;
    return *this;
}

Logger & Logger::operator<<(const string &str) {
    size_t nlIndex;
    if ((nlIndex = str.find('\n')) != string::npos) {
        m->oss << str.substr(0, nlIndex);
        log(m->oss.str());
        m->oss.str("");
        m->oss.clear();
        operator<<(str.substr(nlIndex + 1));
        return *this;
    }
    m->oss << str;
    return *this;
}

//Logger & operator<<(const char *str) {
//    m->oss << str;
//    return *this;
//}

Logger & Logger::operator<<(char c) {
    if (c == '\n') {
        log(m->oss.str());
        m->oss.str("");
        m->oss.clear();
        return *this;
    }
    m->oss << c;
    return *this;
}

Logger & Logger::operator<<(uint8_t c) {
    m->oss << c;
    return *this;
}

Logger & Logger::operator<<(int8_t c) {
    m->oss << c;
    return *this;
}

Logger & Logger::operator<<(int16_t i) {
    m->oss << i;
    return *this;
}

Logger & Logger::operator<<(uint16_t i) {
    m->oss << i;
    return *this;
}

Logger & Logger::operator<<(int32_t i) {
    m->oss << i;
    return *this;
}

Logger & Logger::operator<<(uint32_t i) {
    m->oss << i;
    return *this;
}

Logger & Logger::operator<<(int64_t i) {
    m->oss << i;
    return *this;
}

Logger & Logger::operator<<(uint64_t i) {
    m->oss << i;
    return *this;
}

Logger & Logger::operator<<(float f) {
    m->oss << f;
    return *this;
}

Logger & Logger::operator<<(double d) {
    m->oss << d;
    return *this;
}

Logger & Logger::operator<<(long double d) {
    m->oss << d;
    return *this;
}

Logger & Logger::operator<<(bool b) {
    m->oss << b;
    return *this;
}

Logger & Logger::operator<<(std::bitset<8> b) {
    m->oss << b;
    return *this;
}

Logger & Logger::operator<<(std::bitset<16> b) {
    m->oss << b;
    return *this;
}

Logger & Logger::operator<<(std::bitset<32> b) {
    m->oss << b;
    return *this;
}

Logger & Logger::operator<<(const char *c) {
    operator<<(string(c));
    return *this;
}

Logger::~Logger() {
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
        delete *it;
    }
}

}
}

