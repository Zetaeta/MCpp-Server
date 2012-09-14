
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>

#include "Logger.hpp"

namespace MCServer {
namespace Logging {

class Handler {
public:
    virtual void handle(Level level, const std::string &message) = 0;
    virtual Handler * clone() = 0;
    virtual ~Handler() {}
};

}
}

#endif
