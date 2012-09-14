
#ifndef UIHANDLER_HPP
#define UIHANDLER_HPP

#include "Handler.hpp"


namespace MCServer {

class MinecraftServer;

namespace Logging {

struct UIHandlerData;

class UIHandler : public Handler {
public:
    UIHandler(MinecraftServer *);
    virtual void handle(Level level, const std::string &message);
    virtual Handler * clone();
private:
    static std::string levelName(Level level);
    static std::string timeString();

    UIHandlerData *m;
};

}
}

#endif
