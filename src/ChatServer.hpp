
#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <Util/stlfwd.hpp>

namespace MCServer {

namespace Entities {
class Player;
}

class CommandSender;

using std::shared_ptr;
using Entities::Player;

class ChatServer {
public:
    void handleChatMessage(const std::string &message, const shared_ptr<Player> &sender);
    void sendChatMessage(const std::string &message, const shared_ptr<Player> &sender);
    std::string getFormat(const std::string &message, const shared_ptr<CommandSender> &sender);
private:
    
};

}

#endif
