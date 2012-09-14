
#ifndef CLIUIMANAGER_HPP
#define CLIUIMANAGER_HPP

#include "../UIManager.hpp"

namespace MCServer {
namespace UI {

struct CliUIManagerData;

class CliUIManager : public UIManager {
public:
    CliUIManager(MinecraftServer *server);

    virtual void println(const std::string &);
    virtual void setup();

private:
    CliUIManagerData *m;
};

}
}

#endif
