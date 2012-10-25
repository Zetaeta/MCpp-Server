
#include <string>
#include <iostream>
#include <string.h>
#include <sstream>
#include <errno.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include <IOStream/SocketOutputStream.hpp>
#include <Util/ErrorHandler.hpp>

#include "NetworkServer.hpp"
#include "logging/Logger.hpp"
#include "ClientConnection.hpp"
#include "Scheduler.hpp"

using IOStream::SocketOutputStream;

namespace MCServer {
namespace Network {

namespace {

void * startNetworkServer(void *serverPtr) {
    NetworkServer *server = (NetworkServer *) serverPtr;
    server->run();
    return NULL;
}

}


using std::string;
USING_LOGGING_LEVEL

struct Connection {
    int socketfd;
    sockaddr_in address;
};

struct NetworkServerData {
    pthread_t thread;
    MinecraftServer *server;
    int sockfd;
    sockaddr_in serverAddress;
    int portNum;

    bool shutdown;

    NetworkServerData() :shutdown(false) {}
};

NetworkServer::NetworkServer(MinecraftServer *server)
    :m(new NetworkServerData()) {
    /*
    * Member initialisation here
    */
    signal(SIGPIPE, SIG_IGN);
    m->server = server;
    

//    pthread_create(&m->thread, NULL, &startNetworkServer, this);
    std::function<void (NetworkServer *)> f = &NetworkServer::run;
    server->getScheduler().startThread(&NetworkServer::run, this);
}

NetworkServer::~NetworkServer() {
    if (!m->shutdown) {
        m->server->getLogger() << WARNING << "NetworkServer being destroyed without being stopped!\n";
        shutdown();
    }
    delete m;
}

void NetworkServer::run() {
    init();
    while (true) {
        sockaddr_in clientAddress;
        int clientSockfd;
        socklen_t clientLength = sizeof(clientAddress);
        clientSockfd = accept(m->sockfd, reinterpret_cast<sockaddr *>(&clientAddress), &clientLength);
        Connection cc = {clientSockfd, clientAddress};
        handleAccept(cc);
    }
}


void NetworkServer::init() {
    m->server->getLogger() <<  "Setting up network... \n";
    //              not local, stream (TCP), default protocol (TCP)
    m->portNum = 25565;
    m->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&m->serverAddress, 0, sizeof(m->serverAddress));
    m->serverAddress.sin_family = AF_INET;
    m->serverAddress.sin_port = htons(m->portNum);
    // Port number (INADDR_ANY = this machine's IP)
    m->serverAddress.sin_addr.s_addr = INADDR_ANY;

    int returned = bind(m->sockfd, reinterpret_cast<sockaddr *>(&m->serverAddress), sizeof(m->serverAddress));
    if (returned != 0) {
        throwException(errno);
    }
    //            Size of connection queue (number of connections that can be waiting while server is handling one.
    listen(m->sockfd, 5);
}

void NetworkServer::handleAccept(const Connection &connection) {
    std::ostringstream oss;
    oss << "Accepted client, socketfd = " << connection.socketfd;
    m->server->getLogger().info(oss.str());
    
    unsigned char packetId;
    if (recv(connection.socketfd, &packetId, 1, MSG_PEEK) < 0) {
        m->server->getLogger().info(string("Error reading packet ID: ") + strerror(errno));
    }
    
    std::ostringstream oss2;
    oss2 << "Packet ID: " << static_cast<int>(packetId);
    m->server->getLogger().info(oss2.str());
    if (packetId == 0xFE) {
        serverListPing(connection);
    }
    else {
        m->server->getLogger() << Logging::INFO << "Starting ClientConnection!\n";
        new ClientConnection(connection.socketfd);
    }
}

void NetworkServer::serverListPing(const Connection &connection) {
//    string hello = "Hello World§64§32";
    Logging::Logger &logger = m->server->getLogger();
    logger.info("serverListPing()");
    SocketOutputStream out(connection.socketfd);
    out << static_cast<uint8_t>(0xFF);
    std::ostringstream output;
    output << m->server->getMotd() << "§" << m->server->getOnlinePlayerCount() << "§" << m->server->getMaxPlayers();
    out << output.str();
    logger << "Sending server list data: " << output.str() << '\n';
    logger.info("serverListPing() ending.");
    close(connection.socketfd);
} 

void NetworkServer::shutdown() {
    m->shutdown = true;
}

} /* namespace Network */
} /* namespace MCServer */
