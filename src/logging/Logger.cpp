
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <bitset>

#include "Logger.hpp"
#include "Handler.hpp"
#include "ConsoleHandler.hpp"
#include "../MinecraftServer.hpp"
#include "../Lock.hpp"

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
    Lock lock;

    static map<string, Logger *> loggers;
};

map<string, Logger *> LoggerData::loggers;

Logger::Logger(std::string name)
 :m(new LoggerData()) {
    m->name = name;
    m->defaultLevel = INFO;
}


void Logger::log(Level level, string message) {
    m->lock.lock();
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
        (*it)->handle(level, message);
    }
    m->lock.unlock();
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
        m->lock.lock();
        m->oss << str.substr(0, nlIndex);
        log(m->oss.str());
        m->oss.str("");
        m->oss.clear();
        m->lock.unlock();
        operator<<(str.substr(nlIndex + 1));
        return *this;
    }
    m->lock.lock();
    m->oss << str;
    m->lock.unlock();
    return *this;
}

//Logger & operator<<(const char *str) {
//    m->oss << str;
//    return *this;
//}

Logger & Logger::operator<<(char c) {
    if (c == '\n') {
        m->lock.lock();
        log(m->oss.str());
        m->oss.str("");
        m->oss.clear();
        m->lock.unlock();
        return *this;
    }
    m->lock.lock();
    m->oss << c;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(uint8_t c) {
    m->lock.lock();
    m->oss << c;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(int8_t c) {
    m->lock.lock();
    m->oss << c;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(int16_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(uint16_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(int32_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(uint32_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(int64_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(uint64_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(float f) {
    m->lock.lock();
    m->oss << f;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(double d) {
    m->lock.lock();
    m->oss << d;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(long double d) {
    m->lock.lock();
    m->oss << d;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(bool b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(std::bitset<8> b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(std::bitset<16> b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(std::bitset<32> b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unlock();
    return *this;
}

Logger & Logger::operator<<(const char *c) {
    operator<<(string(c));
    return *this;
}

Logger & Logger::operator<<(std::ios_base & (*modifier)(std::ios_base &)) {
    m->lock.lock();
    m->oss << modifier;
    m->lock.unlock();
    return *this;
}

Logger::~Logger() {
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
        delete *it;
    }
}

}
}

