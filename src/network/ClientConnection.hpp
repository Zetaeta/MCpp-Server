
#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

namespace MCServer {
namespace Network {

struct ClientConnectionData;

class ClientConnection {
public:
    ClientConnection(int socketfd);
    void run();
    void stop();
protected:
    void init();
private:
    ClientConnectionData *m;
};

}
}

#endif
