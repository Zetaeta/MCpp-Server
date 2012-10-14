
#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include <string>

namespace MCServer {
namespace Network {

#ifndef PACKET_HPP
class Packet;
#endif

struct ClientConnectionData;

class ClientConnection {
public:
    ClientConnection(int socketfd);
    void run();
    void shutdown();
    void send(Packet &);
    bool isBlocking();
    void setBlocking(bool);

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
protected:
    void init();
private:
//    void handleEncryptionResponse();
    void setupCrypto(std::string);
    void sendWorld();
    // Login stuff after encryption has been enabled.
    void finishLogin();

    ClientConnectionData *m;
};

}
}

#endif
