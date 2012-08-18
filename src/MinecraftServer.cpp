/*
 * MinecraftServer.cpp
 *
 *  Created on: 3 Aug 2012
 *      Author: daniel
 */

#include <iostream>
#include <exception>
#include <cstdlib>

#include "MinecraftServer.hpp"
#include "network/NetworkServer.hpp"
#include "logging/Logger.hpp"

namespace MCServer {

//using std::thread;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using Network::NetworkServer;
using Logging::Logger;

struct MinecraftServerData {
    bool shutdown;
    NetworkServer *networkServer;
    Logger *logger;
};

MinecraftServer::MinecraftServer()
    :m(new MinecraftServerData())
{
    m->logger = &Logger::getLogger("Minecraft");
}

MinecraftServer::~MinecraftServer() {
    if (!m->shutdown) {
        shutdown();
    }
    delete m;
}

void MinecraftServer::run() {
    try {
        init();
        while (true) {
            tick();
        }
    }
    catch (std::exception &e) {
        cerr << "Unhandled exception in server: " << e.what() << endl;
    }
    catch (const char *str) {
        cerr << "Unhandled (const char *) exception in server: " << str << endl;
    }
    catch (std::string &str) {
        cerr << "Unhandled (string) exception in server: " << str << endl;
    }

    try {
        shutdown();
    }
    catch (std::exception &e) {
        cerr << "Error shutting down: " << e.what() << endl;
        exit(1);
    }
    catch (const char *str) {
        cerr << "Error shutting down: " << str << endl;
        exit(1);
    }
    catch (std::string &str) {
        cerr << "Error shutting down: " << str << endl;
        exit(1);
    }
    
    
}

void MinecraftServer::init() {
    m->logger->info("Starting MC++-Server...");
    m->logger->info("Server version: " + getVersion());
    m->networkServer = new NetworkServer(this);
    
}

void MinecraftServer::tick() {

}

void MinecraftServer::shutdown() {
    
}

string MinecraftServer::getVersion() {
    return "0.1";
}

NetworkServer & MinecraftServer::getNetworkServer() {
    return *m->networkServer;
}

Logger & MinecraftServer::getLogger() {
    return *m->logger;
}

//boost::asio::io_service & MinecraftServer::getIoService() {
//    return m->ioService;
//}

} /* namespace MCServer */
