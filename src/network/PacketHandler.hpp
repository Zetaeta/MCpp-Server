
#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include "Packet.hpp"

namespace MCServer {

#ifndef MINECRAFTSERVER_HPP
class MinecraftServer;
#endif

namespace Network {

#ifndef SOCKETINPUTSTREAM_HPP
class SocketInputStream;
#endif

class PacketHandler {
public:
    static void initialise(MinecraftServer *server);

    static Packet encryptionRequest();
//    static void handleEncryptionResponse(SocketInputStream &);
private:
    PacketHandler();

    static MinecraftServer *server;
};

}
}

#endif
