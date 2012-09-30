
#include "LoggerStreamBuf.hpp"
#include "Logger.hpp"

using std::streambuf;

namespace MCServer {
namespace Logging {

LoggerStreamBuf::LoggerStreamBuf(Logger &logger, Level level)
:log(logger), level(level) {
}

streambuf::int_type LoggerStreamBuf::overflow(streambuf::int_type c) {
    if (c != EOF) {
        log << level << char(c);
    }
    return c;
}

}
}
