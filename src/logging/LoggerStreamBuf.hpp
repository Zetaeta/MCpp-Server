
#ifndef LOGGERSTREAMBUF_HPP
#define LOGGERSTREAMBUF_HPP

#include <streambuf>

#include "Level.hpp"

namespace MCServer {
namespace Logging {

class Logger;

class LoggerStreamBuf : public std::streambuf {
public:
    LoggerStreamBuf(Logger &logger, Level level = INFO);
protected:
    int_type overflow(int_type c);
private:
    Logger &log;
    Level level;
};

}
}

#endif
