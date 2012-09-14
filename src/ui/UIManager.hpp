
#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <string>

namespace MCServer {

class MinecraftServer;

namespace UI {

class UIManager {
public:
    virtual void println(const std::string &) = 0;
    virtual void setup() = 0;
    virtual ~UIManager() {}

    static UIManager & init(MinecraftServer *server, const std::string &uiName);

private:
    static UIManager *uiManager;

    static UIManager & setupCli(MinecraftServer *server);
};

}
}

#endif
