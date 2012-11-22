
#ifndef COMMANDSENDER_HPP
#define COMMANDSENDER_HPP

#include <Util/stlfwd.hpp>

namespace MCServer {

class CommandSender {
public:
    virtual void sendMessage(const std::string &) = 0;
    virtual std::string getName() const = 0;
    virtual ~CommandSender() = default;
};

}

#endif
