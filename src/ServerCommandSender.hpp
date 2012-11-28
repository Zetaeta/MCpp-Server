
#ifndef SERVERCOMMANDSENDER_HPP
#define SERVERCOMMANDSENDER_HPP

#include "CommandSender.hpp"

namespace MCServer {

class ServerCommandSender : public CommandSender {
public:
    virtual void sendMessage(const std::string &);
    virtual std::string getName() const;
    virtual ~ServerCommandSender() {};
};

}

#endif
