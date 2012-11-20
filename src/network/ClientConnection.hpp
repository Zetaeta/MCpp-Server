
#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP


#include <Util/stlfwd.hpp>

namespace MCServer {

class Chunk;

namespace Entities {
class Player;
}

namespace Network {

class Packet;
enum PacketType : uint8_t;

struct ClientConnectionData;

class ClientConnection {
public:
    ClientConnection(int socketfd);
    void run();

    /**
     Handles player disconnection and connection removal (removing references to player, closing socket etc.)
     Does NOT actually kick player, assumes already kicked or quitted.
     */
    void shutdown();
    void send(Packet &);
    bool isBlocking();
    void setBlocking(bool);
    void sendKeepAlive();
    void sendMessage(const std::string &message);
    void sendChunk(const Chunk &);


    /**
     * @param output Output buffer to put read value into, can be NULL in which case memory is allocated and must be delete[]'d
     */
    uint8_t * read(uint8_t *output, size_t length);

    /**
     * Returned value must be delete[]'d.
     */
    uint8_t * read(size_t length);

    Packet & readPacket(Packet &packet, size_t length);
    Packet readPacket(size_t length);

    ~ClientConnection();

    friend class Entities::Player;
protected:
    void init();
private:
//    void handleEncryptionResponse();
    void setupCrypto(std::string);
    void sendWorld();
    void sendSpawnPosition();
    void handlePacket(PacketType);
    void receiveClientSettings();
    void receivePlayerPositionAndLook();
    void receivePlayerPosition();
    void receivePlayerAbilities();
    void receivePlayerLook();
    void receiveChatMessage();

    void updatePosition();

    // Login stuff after encryption has been enabled.
    void finishLogin();

    ClientConnectionData *m;
};

}
}

#endif

