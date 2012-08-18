/*
 * NetworkServer.cpp
 *
 *  Created on: 4 Aug 2012
 *      Author: daniel
 */

#include <string>
#include <iostream>

#include <pthread.h>

#include "NetworkServer.hpp"

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
//    thread *thread;
    pthread_t thread;
//    tcp::acceptor acceptor;
    MinecraftServer *minecraftServer;

//    NetworkServerData(MinecraftServer *minecraftServer)
//        :socket(minecraftServer.getIoService(), tcp::endpoint(tcp::v4(), 25565)) {
//        
//    }
};

NetworkServer::NetworkServer(MinecraftServer *minecraftServer)
    :m(new NetworkServerData()) {
    /*
    * Member initialisation here
    */
    
    

    // Start the server thread.
    //m->thread = new thread(&NetworkServer::run, this);
    pthread_create(&m->thread, NULL, &startNetworkServer, this);
}

NetworkServer::~NetworkServer() {

}

void NetworkServer::run() {
    init();
//    while (true) {
//        tcp::socket *socket = new tcp::socket(ioService);
//        m->acceptor.accept(*socket);
//        acceptClient(socket);
//    }
}

//void NetworkServer::acceptClient(tcp::socket *socket) {
    
//}

void NetworkServer::init() {
    cout << "Setting up network... "
}

} /* namespace Network */
} /* namespace MCServer */
