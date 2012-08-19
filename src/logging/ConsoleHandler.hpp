
#ifndef CONSOLEHANDLER_HPP
#define CONSOLEHANDLER_HPP

#include "Handler.hpp"


namespace MCServer {

class MinecraftServer;

namespace Logging {

struct ConsoleHandlerData;

class ConsoleHandler : public Handler {
public:
    ConsoleHandler(MinecraftServer *);
    virtual void handle(Level level, std::string message);
    virtual Handler * clone();
private:
    static std::string levelName(Level level);
    static std::string timeString();

    ConsoleHandlerData *m;
};

}
}

#endif
