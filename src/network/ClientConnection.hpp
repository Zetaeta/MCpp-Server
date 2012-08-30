
#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include <string>

namespace MCServer {
namespace Network {

struct ClientConnectionData;

class ClientConnection {
public:
    ClientConnection(int socketfd);
    void run();
    void shutdown();
    void send(Packet &);

    ~ClientConnection();
protected:
    void init();
private:
//    void handleEncryptionResponse();
    void setupCrypto(std::string);

    ClientConnectionData *m;
};

}
}

#endif
