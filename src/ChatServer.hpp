
#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <Util/stlfwd.hpp>

namespace MCServer {

namespace Entities {
class Player;
}

using std::shared_ptr;

class ChatServer {
public:
    void handleChatMessage(const std::string &message, const shared_ptr<Entities::Player> &sender);
    std::string getFormat(const std::string &message, const shared_ptr<Entities::Player> &sender);
private:
    
};

}

#endif
