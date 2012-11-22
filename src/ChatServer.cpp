
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>

#include <Util/StringUtils.hpp>

#include "ChatServer.hpp"
#include "MinecraftServer.hpp"
#include "game/entity/Player.hpp"
#include "command/CommandManager.hpp"
#include "event/EventManager.hpp"
#include "event/entity/player/PlayerChatEvent.hpp"

namespace MCServer {

using std::string;
using std::vector;
using std::for_each;
using std::shared_ptr;

using Entities::Player;
using Events::PlayerChatEvent;

void ChatServer::sendChatMessage(const string &message, const shared_ptr<Player> &sender) {
    string format = getFormat(message, sender);
    PlayerChatEvent evt(message, sender);
    MinecraftServer::server().getEventManager().dispatchEvent(evt);
    const vector<shared_ptr<Player>> &players = MinecraftServer::server().getPlayers();
    for(const shared_ptr<Player> player : players) {
        player->sendMessage(format);
    }
}

void ChatServer::handleChatMessage(const string &message, const shared_ptr<Player> &sender) {
    if (Util::startsWith(message, "/")) {
        MinecraftServer::server().getCommandManager().dispatchCommand(message.substr(1), sender);
    }
    else {
        sendChatMessage(message, sender);
    }
}

string ChatServer::getFormat(const string &message, const shared_ptr<CommandSender> &sender) {
    std::ostringstream format;
    format << '<' << sender->getName() << "> " << message;
    return format.str();
}

}
