
#include <string>

#include "PacketHandler.hpp"
#include "Packets.hpp"
#include "SocketInputStream.hpp"
#include "../MinecraftServer.hpp"
#include "../logging/Logger.hpp"

namespace MCServer {
namespace Network {

using std::string;

MinecraftServer *PacketHandler::server;

void PacketHandler::initialise(MinecraftServer *server) {
    PacketHandler::server = server;
}

Packet PacketHandler::encryptionRequest() {
    Logging::Logger &log = server->getLogger();
    log << "PacketHandler::encryptionRequest()\n";
    Packet pack;
    pack << PACKET_ENCRYPTION_KEY_REQUEST;
    log << "Server id: " << server->getServerId() << '\n';
    pack << server->getServerId();
    const string &pk = server->getPublicKey();
    pack << static_cast<int16_t>(pk.size());
    pack.add(pk.c_str(), pk.size());
    const string &vt = server->getVerifyToken();
    pack << static_cast<int16_t>(vt.size());
    pack.add(vt.c_str(), vt.size());
    log << "Finished making encryption request packet!\n";
    return pack;
}

}
}
