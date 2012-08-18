
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace MCServer {
namespace Logging {


enum Level {
    FINEST,
    FINE,
    INFO,
    WARNING,
    SEVERE
}

struct LoggerData;

class Logger {
public:
    void log(Level level, std::string message);
    inline void log(std::string message) {
        log(defaultLevel, message);
    }
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

    static Logger & getLogger(std::string name);
    static void destroyLogger(Logger &logger);
private:
    Logger(std::string);

    LoggerData *m;
};

}
}

#endif // LOGGER_HPP
