
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <stdint.h>
#include <ios>
#include <bitset>

#include "Level.hpp"

namespace MCServer {
namespace Logging {

struct LoggerData;
class Handler;

class Logger {
public:
    void log(Level level, const std::string &message);
    void log(const std::string &message);
    inline void finest(const std::string &message) {
        log(FINEST, message);
    }
    inline void fine(const std::string &message) {
        log(FINE, message);
    }
    inline void info(const std::string &message) {
        log(INFO, message);
    }
    inline void warning(const std::string &message) {
        log(WARNING, message);
    }
    inline void severe(const std::string &message) {
        log(SEVERE, message);
    }
    inline void destroy() {
        destroyLogger(*this);
    }

    void lock();
    void unLock();

    Logger & operator<<(Level);
    Logger & operator<<(const std::string &);
    Logger & operator<<(char);
    Logger & operator<<(uint8_t);
    Logger & operator<<(int8_t);
    Logger & operator<<(int16_t);
    Logger & operator<<(uint16_t);
    Logger & operator<<(int32_t);
    Logger & operator<<(uint32_t);
    Logger & operator<<(int64_t);
    Logger & operator<<(uint64_t);
    Logger & operator<<(float);
    Logger & operator<<(double);
    Logger & operator<<(long double);
    Logger & operator<<(const char *);
    Logger & operator<<(bool);
    Logger & operator<<(std::bitset<8>);
    Logger & operator<<(std::bitset<16>);
    Logger & operator<<(std::bitset<32>);
    Logger & operator<<(std::ios_base & (*)(std::ios_base &));

    void addHandler(Handler *);
    void setDefaultLevel(Level level);
    // Set current level (reset every line)
    void setLevel(Level level);
    // Reset current level to default level
    void resetLevel();
    ~Logger();

    static Logger & getLogger(const std::string &name, bool init = true);
    static void destroyLogger(Logger &logger);
private:
    Logger(const std::string &);

    LoggerData *m;
};

extern Logger &log;

}
}

#endif // LOGGER_HPP
