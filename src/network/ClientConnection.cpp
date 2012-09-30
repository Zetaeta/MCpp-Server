
#include <iostream>
#include <string>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>

#include "ClientConnection.hpp"
#include "SocketStream.hpp"
#include "PlainSocketStream.hpp"
#include "EncryptedSocketStream.hpp"
#include "logging/Logger.hpp"
#include "MinecraftServer.hpp"
#include "PacketHandler.hpp"
#include "Packets.hpp"
#include "game/entity/Player.hpp"


namespace MCServer {
namespace Network {

using std::string;
using Logging::Logger;
USING_LOGGING_LEVEL
using Entities::Player;

namespace {

void * startClientConnection(void * cc_) {
    MinecraftServer::getServer().getLogger() << "startClientConnection!\n";
    ClientConnection *cc = static_cast<ClientConnection *>(cc_);
    cc->run();
    return NULL;
}

}

struct ClientConnectionData {
    MinecraftServer &server;
    int socketfd;
    
    pthread_t threadId;
    bool shutdown;
    bool encrypted;
    string sharedSecret;
    EVP_CIPHER_CTX encryptor, decryptor;

    PlainSocketStream pss;
    EncryptedSocketStream ess;
    inline SocketStream & ss() {
        if (encrypted) {
            return ess;
        }
        else {
            return pss;
        }
    }

    ClientConnectionData(int sfd)
    :server(MinecraftServer::getServer()), socketfd(sfd), shutdown(false), encrypted(false), pss(sfd) {}

};

ClientConnection::ClientConnection(int socketfd) 
    :m(new ClientConnectionData(socketfd)) {
//    setBlocking(false);
    Logger &log = m->server.getLogger();
    log << "ClientConnection(int)!\n";
    log << "About to start thread\n";
    pthread_create(&m->threadId, NULL, &startClientConnection, this);
}

ClientConnection::~ClientConnection() {
    if (!m->shutdown) {
        m->server.getLogger() << WARNING << "ClientConnection being destroyed without being stopped.\n";
        shutdown();
    }
    delete m;
}

void ClientConnection::init() {
    Logger &log = m->server.getLogger();
    log << "ClientConnection::init\n";

    string username;
    string serverHost;
    int serverPort;

    { // Scope for ss.
    SocketStream &ss = m->ss();
    uint8_t packetHeader;
    ss >> packetHeader;
    if (packetHeader != 0x02) {
        shutdown();
    }
    uint8_t protocolVersion;
    log << "About to read packet\n";
    
    ss >> protocolVersion >> username >> serverHost >> serverPort;
    log << "Read packet!\n";
    m->server.getLogger() << "User " << username << " attempting to log into " << serverHost << ":" << serverPort << " with protocol version " << static_cast<int>(protocolVersion) << '\n';

    m->server.getLogger() << "About to send encryption request!\n";
    Packet p = PacketHandler::encryptionRequest();
    ss << p;
    m->server.getLogger() << "Sent encryption request!\n";

    uint8_t packetId;
    ss >> packetId;
    if (packetId != PACKET_ENCRYPTION_KEY_RESPONSE) {
        m->server.getLogger() << "Not an encryption key response packet: " << static_cast<uint16_t>(packetId) << '\n';
        // TODO: Handle error
        return;
    }
    m->server.getLogger() << "Recived encryption key response packet header!\n";
    short ssLength;
    ss >> ssLength;
    uint8_t *sharedSecret = ss.read<uint8_t>(ssLength);
    short vtLength;
    ss >> vtLength;
    uint8_t *verifyToken = ss.read<uint8_t>(vtLength);
    if (ssLength > 1023 || vtLength > 1023) {
        // TODO: Error handling
        return;
    }
    uint8_t buffer[1024];
    int verifyLen = RSA_private_decrypt(vtLength, verifyToken, buffer, m->server.getRsa(), RSA_PKCS1_PADDING);
    if (verifyLen != 4) {
        m->server.getLogger() << WARNING << "Invalid verify token length: " << verifyLen;
        // TODO: Error handling
        return;
    }
    if (string(reinterpret_cast<const char *>(buffer)) != m->server.getVerifyToken()) {
        m->server.getLogger() << INFO << "Disconnecting user: " << username << ": incorrect verify token.\n";
        return;
    }

    int secretLen = RSA_private_decrypt(ssLength, sharedSecret, buffer, m->server.getRsa(), RSA_PKCS1_PADDING);
    m->server.getLogger() << INFO << "Decrypted shared secret from user " << username << ": " << string(reinterpret_cast<const char *>(buffer), secretLen) << '\n';

    setupCrypto(string(reinterpret_cast<const char *>(buffer), secretLen));
    } // End scope for ss.

    SocketStream &ss = m->ss();
    uint8_t clientStatusId;
    uint8_t payload;
    ss >> clientStatusId;
    if (clientStatusId != PACKET_CLIENT_STATUS) {
        log << WARNING << "Invalid client status header: 0x" << std::hex << static_cast<uint16_t>(clientStatusId) << '\n' << std::dec;
        return;
    }
    ss >> payload;
    if (payload) {
        log << WARNING << "Invalid client status payload: 0x" << std::hex << static_cast<uint16_t>(payload) << '\n' << std::dec;
        return;
    }

    Player *player = new Player(username);
    Packet login;
    login << player->getId();
    login << m->server.getLevelType() << m->server.getDefaultGameMode() << m->server.getWorldType() << m->server.getDifficulty();
    login << uint8_t(0) << uint8_t(m->server.getMaxPlayers());
    ss << login;
}

/**
 * Sets up the connection encryption for secure communication.
 */
void ClientConnection::setupCrypto(std::string sharedSecret) {
    uint8_t key[16], iv[16];

    memcpy(key, sharedSecret.c_str(), 16);
    memcpy(iv, sharedSecret.c_str(), 16);

    EVP_CIPHER_CTX_init(&m->encryptor);
    EVP_CIPHER_CTX_init(&m->decryptor);

    EVP_EncryptInit_ex(&m->encryptor, EVP_aes_128_cfb8(), NULL, key, iv);
    EVP_DecryptInit_ex(&m->decryptor, EVP_aes_128_cfb8(), NULL, key, iv);

    // Empty 0xFC packet.
    Packet pack;
    pack << PACKET_ENCRYPTION_KEY_RESPONSE << 0; // 1 int = 2 shorts.

    m->ss() << pack;

    m->ess = EncryptedSocketStream(m->socketfd, &m->encryptor, &m->decryptor);
    m->encrypted = true;
//    setBlocking(false);
}

void ClientConnection::run() {
    m->server.getLogger() << "ClientConnection::run\n";
    init();
    // Read-handle loop here.
}

/*
void ClientConnection::send(Packet &pack) {
    if (!m->encrypted) {
        m->ss() << pack;
        return;
    }
    else {
        int outLength = pack.size() + m->encryptor.block_size - 1;
        uint8_t *output = new uint8_t[outLength];
        EVP_EncryptUpdate(&m->encryptor, output, &outLength, pack.getBytes(), pack.size()); 
        ss.write(output, outLength);
        return;
    }
}

uint8_t * ClientConnection::read(uint8_t *output, size_t length) {
    if (!m->encrypted) {
        ::read(m->socketfd, output, length);
    }
    else {
        int inLength = length;
        uint8_t input = new uint8_t[outLength];
        EVP_DecryptUpdate(&m->decryptor, output, &outLength, 
    }
    return output;
}*/

/**
 * Shuts down the connection, freeing any resources and removing any references to it.
 */
void ClientConnection::shutdown() {
    m->shutdown = true;
}

bool ClientConnection::isBlocking() {
    return !(fcntl(m->socketfd, F_GETFL) & O_NONBLOCK);
}

void ClientConnection::setBlocking(bool blocking) {
    int fl = fcntl(m->socketfd, F_GETFL);
    if (blocking) {
        fl &= ~O_NONBLOCK;
    }
    else {
        fl |= O_NONBLOCK;
    }
    fcntl(m->socketfd, F_SETFL, fl);
}

}
}

