
#ifndef CONSOLEHANDLER_HPP
#define CONSOLEHANDLER_HPP

#include "Handler.hpp"

namespace MCServer {
namespace Logging {

class ConsoleHandler : public Handler {
public:
    virtual void handle(Level level, std::string message);
private:
    static std::string levelName(Level level);
    static std::string timeString();
};

}
}

#endif
