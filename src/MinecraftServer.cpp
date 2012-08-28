/*
 * MinecraftServer.cpp
 *
 *  Created on: 3 Aug 2012
 *      Author: daniel
 */

#include <iostream>
#include <exception>
#include <cstdlib>

#include <unistd.h>

#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <openssl/rc4.h>

#include "MinecraftServer.hpp"
#include "network/NetworkServer.hpp"
#include "logging/Logger.hpp"
#include "ConsoleReader.hpp"

namespace MCServer {

//using std::thread;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

using Network::NetworkServer;
using Logging::Logger;
using Logging::Level;
using namespace MCServer::Logging;

struct MinecraftServerData {
    bool shutdown;
    NetworkServer *networkServer;
    Logger *logger;
    ConsoleReader *consoleReader;

    // Crypto stuff
    RSA *rsa;
    X509 *x509;
    EVP_PKEY *pk;
    string serverId;
    string publicKey;
    string encryptionBytes;
    

    static MinecraftServer *server;
};
MinecraftServer * MinecraftServerData::server;

MinecraftServer::MinecraftServer()
    :m(new MinecraftServerData())
{
    MinecraftServerData::server = this;
    m->logger = &Logger::getLogger("Minecraft");
    m->consoleReader = new ConsoleReader(this);

    m->rsa = RSA_generate_key(1024, 17, 0, 0);

    m->x509 = X509_new();
    m->pk = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(m->pk, m->rsa);
    X509_set_version(m->x509, 0);
    X509_set_pubkey(m->x509, m->pk);

    int len;
    uint8_t *buf = NULL;
    len = i2d_X509(m->x509, &buf);
    
    // Wat.
    m->publicKey = string(reinterpret_cast<char *>(buf), len - 36);
    OPENSSL_free(buf);
    
    string encryptionByteCharacters = "abcedfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=";
    string hexDigits = "0123456789abcdef";
    char serverId_[17];
    char *serverId = serverId_;
    char encryptionBytes[5];
    for (int i=0; i<16; ++i) { // Server ID random generation
        serverId[i] = hexDigits[rand() % hexDigits.size()];
    }
    serverId[16] = '\0';
    for (int i=0; i<4; ++i) { // Encryption bytes random generation.
        encryptionBytes[i] = encryptionByteCharacters[rand() % encryptionByteCharacters.size()];
    }
    encryptionBytes[4] = '\0';
    if (!userValidationEnabled()) {
        serverId = "-";
    }
    *m->logger << INFO << "Server ID: " << serverId;
    m->serverId = serverId;
    m->encryptionBytes = encryptionBytes;
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

void MinecraftServer::dispatchConsoleCommand(string command) {
    m->logger->info("Dispatching command " + command);
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

ConsoleReader & MinecraftServer::getConsoleReader() {
    return *m->consoleReader;
}

std::string MinecraftServer::getMotd() {
    return "MC++-Server. Meow";
}

int MinecraftServer::getMaxPlayers() {
    return 1024;
}

int MinecraftServer::getOnlinePlayerCount() {
    return 2048;
}


MinecraftServer & MinecraftServer::getServer() {
    return *MinecraftServerData::server;
}

} /* namespace MCServer */
