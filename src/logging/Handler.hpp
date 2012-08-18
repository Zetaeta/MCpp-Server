
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>

namespace MCServer {
namespace Logging {

class Handler {
public:
    virtual void handle(Level level, std::string message) = 0;
    virtual ~Handler() {}
};

}
}

#endif
