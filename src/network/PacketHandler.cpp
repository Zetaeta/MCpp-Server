
#include "PacketHandler.hpp"
#include "Packets.hpp"

namespace MCServer {
namespace Network {

static MinecraftServer &PacketHandler::server = MinecraftServer::getServer();

Packet PacketHandler::encryptionRequest() {
    Packet pack;
    pack << server.getServerId();
    const string &pk = server.getPublicKey();
    pack.addBytes(pk.c_str(), pk.size());
    const string &eb = server.getEncryptionBytes();
    pack.addBytes(eb.c_str(), eb.size());
    return pack;
}

void PacketHandler::handleEncryptionResponse(SocketInputStream &ss) {
    uint8_t packetId;
    ss >> packetId;
    if (packetId != PACKET_ENCRYPTION_RESPONSE) {
        
    }
}

}
}
