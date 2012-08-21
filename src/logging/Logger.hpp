
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <stdint.h>

#include "Level.hpp"

namespace MCServer {
namespace Logging {

struct LoggerData;
class Handler;

class Logger {
public:
    void log(Level level, std::string message);
    void log(std::string message);
    inline void finest(std::string message) {
        log(FINEST, message);
    }
    inline void fine(std::string message) {
        log(FINE, message);
    }
    inline void info(std::string message) {
        log(INFO, message);
    }
    inline void warning(std::string message) {
        log(WARNING, message);
    }
    inline void severe(std::string message) {
        log(SEVERE, message);
    }
    inline void destroy() {
        destroyLogger(*this);
    }

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
//    Logger & operator<<(const char *);
    Logger & operator<<(bool);

    void addHandler(Handler *);
    void setDefaultLevel(Level level);
    ~Logger();

    static Logger & getLogger(std::string name);
    static void destroyLogger(Logger &logger);
private:
    Logger(std::string);

    LoggerData *m;
};

}
}

#endif // LOGGER_HPP
