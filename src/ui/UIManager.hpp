
#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <string>

namespace MCServer {
namespace UI {

class UIManager {
public:
    virtual void println(const std::string &) = 0;
    virtual ~UIManager() {}
};

}
}

#endif
