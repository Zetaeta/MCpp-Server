
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <bitset>

#include "Logger.hpp"
#include "Handler.hpp"
#include "UIHandler.hpp"
#include "MinecraftServer.hpp"
#include "ReentrantLock.hpp"

using std::vector;
using std::string;
using std::map;
using std::ostringstream;

namespace MCServer {
namespace Logging {


struct LoggerData {
    string name;
    Level defaultLevel;
    Level currentLevel;
    vector<Handler *> handlers;
    ostringstream oss;
    ReentrantLock lock;

    static map<string, Logger *> loggers;
};

map<string, Logger *> LoggerData::loggers;

Logger::Logger(const std::string &name)
 :m(new LoggerData()) {
    m->name = name;
    m->defaultLevel = INFO;
}


void Logger::log(Level level, const string &message) {
    m->lock.lock();
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
        (*it)->handle(level, message);
    }
    m->lock.unLock();
}

void Logger::log(const string &message) {
    log(m->currentLevel, message);
}

void Logger::addHandler(Handler *handler) {
    m->handlers.push_back(handler->clone());
}

Logger & Logger::getLogger(const string &name, bool init) {
    Logger *log;
    if ((log = LoggerData::loggers[name]) != 0) {
        return *log;
    }
    log = new Logger(name);
    LoggerData::loggers[name] = log;
    if (init) {
        log->addHandler(new UIHandler(&MinecraftServer::getServer()));
    }
    return *log;
}

void Logger::destroyLogger(Logger &logger) {
    LoggerData::loggers.erase(logger.m->name);
    delete &logger;
}

void Logger::setDefaultLevel(Level level) {
    m->defaultLevel = level;
}

void Logger::setLevel(Level level) {
    m->currentLevel = level;
}

void Logger::resetLevel() {
    m->currentLevel = m->defaultLevel;
}

void Logger::lock() {
    m->lock.lock();
}

void Logger::unLock() {
    m->lock.unLock();
}

Logger & Logger::operator<<(Level level) {
    m->currentLevel = level;
    return *this;
}

Logger & Logger::operator<<(const string &str) {
    size_t nlIndex;
    if ((nlIndex = str.find('\n')) != string::npos) {
        m->lock.lock();
        m->oss << str.substr(0, nlIndex);
        log(m->oss.str());
        resetLevel();
        m->oss.str("");
        m->oss.clear();
        m->lock.unLock();
        operator<<(str.substr(nlIndex + 1));
        return *this;
    }
    m->lock.lock();
    m->oss << str;
    m->lock.unLock();
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
        resetLevel();
        m->oss.str("");
        m->oss.clear();
        m->lock.unLock();
        return *this;
    }
    m->lock.lock();
    m->oss << c;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(uint8_t c) {
    m->lock.lock();
    m->oss << c;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(int8_t c) {
    m->lock.lock();
    m->oss << c;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(int16_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(uint16_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(int32_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(uint32_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(int64_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(uint64_t i) {
    m->lock.lock();
    m->oss << i;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(float f) {
    m->lock.lock();
    m->oss << f;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(double d) {
    m->lock.lock();
    m->oss << d;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(long double d) {
    m->lock.lock();
    m->oss << d;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(bool b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(std::bitset<8> b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(std::bitset<16> b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(std::bitset<32> b) {
    m->lock.lock();
    m->oss << b;
    m->lock.unLock();
    return *this;
}

Logger & Logger::operator<<(const char *c) {
    operator<<(string(c));
    return *this;
}

Logger & Logger::operator<<(std::ios_base & (*modifier)(std::ios_base &)) {
    m->lock.lock();
    m->oss << modifier;
    m->lock.unLock();
    return *this;
}

Logger::~Logger() {
    for (auto it = m->handlers.begin(); it != m->handlers.end(); ++it) {
        delete *it;
    }
}

Logger &log = Logger::getLogger("Minecraft", false);

}
}

