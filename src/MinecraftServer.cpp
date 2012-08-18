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

namespace MCServer {

//using std::thread;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using Network::NetworkServer;

struct MinecraftServerData {
    bool shutdown;
    NetworkServer *networkServer;
//    boost::asio::io_service ioService;
};

MinecraftServer::MinecraftServer()
    :m(new MinecraftServerData())
{

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
    
/*    catch (char c) {
        cerr << "Unhandled (char) exception in server: " << str << endl;
        ++failureCount;
        goto finish;
    }
    catch (short s) {
        cerr << "Unhandled (short) exception in server: " << s << endl;
        ++failureCount;
        goto finish;
    }
    catch (int i) {
        cerr << "Unhandled (int) exception in server: " << i << endl;
        ++failureCount;
        goto finish
    }
    catch (long long ll) {
        cerr << "Unhandled (really long) exception in server: " << ll << endl;
        ++failureCount;
        goto finish;
    }
    catch (float f) {
        cerr << "Unhandled (float) exception in server: " << f << endl;
        ++failureCount;
        goto finish;
    }
    catch (double d) {
        cerr << "Unhandled (double) exception in server: " << f << endl;
        ++failureCount;
        goto finish;
    }
    catch (bool b) {
        cerr << "Unhandled (bool) exception in server: " << b << '\n';
        ++failureCount;
        cerr << "Who the fuck thought it would be a good idea to throw a bool? ¬_¬" << endl;;
        goto finish;
    }
    catch (...) {
        cerr << "Unhandled exception of unknown type in server.\n";
        ++failureCount;
        goto finish;
    } */
    
}

void MinecraftServer::init() {
    cout << "Starting MC++-Server...\n";
    cout << "Server version: " << getVersion() << '\n';
    m->networkServer = new NetworkServer(this);
    
}

void MinecraftServer::tick() {

}

void MinecraftServer::shutdown() {
    
}

string MinecraftServer::getVersion() {
    return "0.1";
}

//boost::asio::io_service & MinecraftServer::getIoService() {
//    return m->ioService;
//}

} /* namespace MCServer */
