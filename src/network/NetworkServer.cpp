/*
 * NetworkServer.cpp
 *
 *  Created on: 4 Aug 2012
 *      Author: daniel
 */

#include <string>
#include <iostream>
#include <string.h>
#include <sstream>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "NetworkServer.hpp"
#include "../logging/Logger.hpp"

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
using std::cout;


struct NetworkServerData {
    pthread_t thread;
    MinecraftServer *server;
    int sockfd;
    sockaddr_in serverAddress;
    int portNum;
};

NetworkServer::NetworkServer(MinecraftServer *minecraftServer)
    :m(new NetworkServerData()) {
    /*
    * Member initialisation here
    */
    
    

    pthread_create(&m->thread, NULL, &startNetworkServer, this);
}

NetworkServer::~NetworkServer() {

}

void NetworkServer::run() {
    init();
    while (true) {
        sockaddr_in clientAddress;
        int clientSockfd;
        socklen_t clientLength = sizeof(clientAddress);
        clientSockfd = accept(m->sockfd, reinterpret_cast<sockaddr *>(&clientAddress), &clientLength);
        handleAccept(clientSockfd, clientAddress);
    }
}


void NetworkServer::init() {
    cout << "Setting up network... \n";
    //              not local, stream (TCP), default protocol (TCP)
    m->portNum = 25565;
    m->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&m->serverAddress, 0, sizeof(m->serverAddress));
    m->serverAddress.sin_family = AF_INET;
    m->serverAddress.sin_port = htons(m->portNum);
    // Port number (INADDR_ANY = this machine's IP)
    m->serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(m->sockfd, reinterpret_cast<sockaddr *>(&m->serverAddress), sizeof(m->serverAddress));
    //            Size of connection queue (number of connections that can be waiting while server is handling one.
    listen(m->sockfd, 5);
}

void NetworkServer::handleAccept(int clientSockfd, sockaddr_in clientAddress) {
    std::ostringstream oss;
    oss << "Accepted client, socketfd = " << clientSockfd;
    m->server->getLogger().info(oss.str());
}

} /* namespace Network */
} /* namespace MCServer */
