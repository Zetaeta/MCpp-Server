
#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include "Packet.hpp"

namespace MCServer {
namespace Network {


class PacketHandler {
public:
    static Packet encryptionRequest();
    static void handleEncryptionResponse(SocketInputStream &);
private:
    PacketHandler();

    static MinecraftServer &server;
};

}
}

#endif
