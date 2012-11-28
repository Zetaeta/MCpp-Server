
#include <string>

#include "ServerCommandSender.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"

namespace MCServer {

using std::string;

void ServerCommandSender::sendMessage(const std::string &msg) {
    MinecraftServer::server().getLogger().info(msg);
}

string ServerCommandSender::getName() const {
    return "Server";
}

}

