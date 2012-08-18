
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

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
