
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>

#include "ChatServer.hpp"
#include "MinecraftServer.hpp"
#include "game/entity/Player.hpp"

namespace MCServer {

using std::string;
using std::vector;
using std::for_each;
using std::shared_ptr;

using Entities::Player;

void ChatServer::handleChatMessage(const string &message, const shared_ptr<Player> &sender) {
    string format = getFormat(message, sender);
    const vector<shared_ptr<Player>> &players = MinecraftServer::server().getPlayers();
    for_each(players.begin(), players.end(), [&] (const shared_ptr<Player> &player) {
        player->sendMessage(format);
    });
}

string ChatServer::getFormat(const string &message, const shared_ptr<Player> &sender) {
    std::ostringstream format;
    format << '<' << sender->getName() << "> " << message;
    return format.str();
}

}
