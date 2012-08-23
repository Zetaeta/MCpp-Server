
#include <string>

#include "ClientConnection.hpp"
#include "SocketStream.hpp"
#include "../logging/Logger.hpp"
#include "../MinecraftServer.hpp"

namespace MCServer {
namespace Network {

using std::string;
using Logging::Logger;

namespace {

void * startClientConnection(void * cc_) {
    MinecraftServer::getServer().getLogger() << "startClientConnection!\n";
    ClientConnection *cc = static_cast<ClientConnection *>(cc_);
    cc->run();
    return NULL;
}

}

struct ClientConnectionData {
    int socketfd;
    SocketStream ss;
    pthread_t threadId;

    ClientConnectionData(int sfd)
    :socketfd(sfd), ss(sfd)
    { }
};

ClientConnection::ClientConnection(int socketfd) 
    :m(new ClientConnectionData(socketfd)) {
    Logger &log = MinecraftServer::getServer().getLogger();
    log << "ClientConnection(int)!\n";
    log << "About to start thread\n";
    pthread_create(&m->threadId, NULL, &startClientConnection, this);
}

void ClientConnection::init() {
    Logger &log = MinecraftServer::getServer().getLogger();
    log << "ClientConnection::init\n";
    SocketStream &ss = m->ss;
    uint8_t packetHeader;
    ss >> packetHeader;
    if (packetHeader != 0x02) {
        stop();
    }
    uint8_t protocolVersion;
    string username;
    string serverHost;
    int serverPort;
    log << "About to read packet\n";
    
    ss >> protocolVersion >> username >> serverHost >> serverPort;
    log << "Read packet!\n";
    MinecraftServer::getServer().getLogger() << "User " << username << " attempting to log into " << serverHost << ":" << serverPort << " with protocol version " << static_cast<int>(protocolVersion) << '\n';
}

void ClientConnection::run() {
    MinecraftServer::getServer().getLogger() << "ClientConnection::run\n";
    init();
    // Read-handle loop here.
}

/**
 * Shuts down the connection, freeing any resources and removing any references to it.
 */
void ClientConnection::stop() {
    
}

}
}
