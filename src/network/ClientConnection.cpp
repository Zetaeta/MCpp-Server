
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include <unistd.h>
#include <fcntl.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <IOStream/ArrayOutputStream.hpp>
#include <IOStream/DeflateOutputStream.hpp>
#include <IOStream/SocketStream.hpp>
#include <IOStream/EncryptedInputStream.hpp>
#include <IOStream/EncryptedOutputStream.hpp>

#include "ClientConnection.hpp"
#include "logging/Logger.hpp"
#include "MinecraftServer.hpp"
#include "PacketHandler.hpp"
#include "Packets.hpp"
#include "game/entity/Player.hpp"
#include "game/ChunkCoordinates.hpp"
#include "game/World.hpp"
#include "game/Chunk.hpp"
#include "game/Point3D.hpp"

using std::string;
using std::vector;
using std::cout;

using IOStream::ArrayOutputStream;
using IOStream::DeflateOutputStream;
using IOStream::SocketStream;
using IOStream::EncryptedInputStream;
using IOStream::EncryptedOutputStream;

namespace MCServer {

using Logging::Logger;
USING_LOGGING_LEVEL
using Entities::Player;

namespace Network {

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
    Player *player;
    int socketfd;
    
    pthread_t threadId;
    bool shutdown;
    bool encrypted;
    string sharedSecret;
    EVP_CIPHER_CTX encryptor, decryptor;

    SocketStream ss;

    ClientConnectionData(int sfd)
    :server(MinecraftServer::getServer()), socketfd(sfd), shutdown(false), encrypted(false), ss(sfd) {}

};

ClientConnection::ClientConnection(int socketfd) 
:m(new ClientConnectionData(socketfd)) {
    Logger &log = m->server.getLogger();
    log << "ClientConnection(int)!\n";
    static bool loadedCryptoStrings = false;
    if (!loadedCryptoStrings) {
        loadedCryptoStrings = true;
        log << "Loading crypto strings\n";
        ERR_load_crypto_strings();
        log << "Loaded crypto strings!\n";
    }
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

    SocketStream &ss = m->ss;
    uint8_t packetHeader;
    ss >> packetHeader;
    if (packetHeader != 0x02) {
        shutdown();
    }
    uint8_t protocolVersion;
    log << "About to read packet\n";
    
    ss >> protocolVersion >> username >> serverHost >> serverPort;
    log << "Read packet!\n";
    m->player = new Player(username);
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
    m->server.getLogger() << "Shared secret length = " << ssLength << "!\n";
    uint8_t *sharedSecret = new uint8_t[ssLength];
    ss.read(sharedSecret, ssLength);
    m->server.getLogger() << "Recived shared secret!\n";
    short vtLength;
    ss >> vtLength;
    m->server.getLogger() << "Verify token length = " << vtLength << "!\n";
    uint8_t *verifyToken = new uint8_t[vtLength];
    ss.read(verifyToken, vtLength);
    m->server.getLogger() << "Recived verify token!\n";
    if (ssLength > 1023 || vtLength > 1023) {
        // TODO: Error handling
        m->server.getLogger() << "(ssLength > 1023 || vtLength > 1023)!\n";
        return;
    }
    uint8_t buffer[1024];
    m->server.getLogger() << "About to decrypt verify token!\n";
    int verifyLen = RSA_private_decrypt(vtLength, verifyToken, buffer, m->server.getRsa(), RSA_PKCS1_PADDING);
    m->server.getLogger() << "Decrypted verify token length: " << verifyLen << '\n';
    if (verifyLen != 4) {
        m->server.getLogger() << WARNING << "Invalid verify token length: " << verifyLen << '\n';
        if (verifyLen < 0) {
            m->server.getLogger() << "Loading crypto strings!\n";
            m->server.getLogger() << "meow\n";
            ERR_load_crypto_strings();
            m->server.getLogger() << "meow\n";
            m->server.getLogger() << WARNING << "Error decrypting verify token: \n";
            const char *errstr = ERR_error_string(ERR_get_error(), NULL);
            if (!errstr) {
                m->server.getLogger() << "unobtainable error message";
            }
            else {
                m->server.getLogger() << errstr;
            }
            m->server.getLogger() << '\n';
        }
        // TODO: Error handling
        shutdown();
        return;
    }
    if (string(reinterpret_cast<const char *>(buffer), 4) != m->server.getVerifyToken()) {
        m->server.getLogger() << INFO << "Disconnecting user: " << username << ": incorrect verify token.\n";
        m->server.getLogger() << "Expected " << m->server.getVerifyToken() << ", got " << string(reinterpret_cast<const char *>(buffer), 4) << '\n';
        shutdown();
        return;
    }

    m->server.getLogger() << "About to decrypt shard secret!\n";
    int secretLen = RSA_private_decrypt(ssLength, sharedSecret, buffer, m->server.getRsa(), RSA_PKCS1_PADDING);
    m->server.getLogger() << "Decrypted shared secret length: " << secretLen << '\n';
    m->server.getLogger() << INFO << "Decrypted shared secret from user " << username << ": " << string(reinterpret_cast<const char *>(buffer), secretLen) << '\n';

    setupCrypto(string(reinterpret_cast<const char *>(buffer), secretLen));
    finishLogin();
}

void ClientConnection::finishLogin() {
    Logger &log = MinecraftServer::server().getLogger();

    SocketStream &ss = m->ss;
    uint8_t clientStatusId;
    uint8_t payload;
    ss >> clientStatusId;
    if (clientStatusId != PACKET_CLIENT_STATUS) {
        log << WARNING << "Invalid client status header: 0x" << std::hex << static_cast<uint16_t>(clientStatusId) << '\n' << std::dec;
//        shutdown();
//        return;
        if (clientStatusId == 0) {
            goto meow;
        }
    }
    ss >> payload;
    if (payload) {
        log << WARNING << "Invalid client status payload: 0x" << std::hex << static_cast<uint16_t>(payload) << '\n' << std::dec;
        shutdown();
        return;
    }
    log << INFO << "Read client status packet!\n";
    meow:

    Packet login;
    login << PACKET_LOGIN_REQUEST;
    login << m->player->getId();
    login << m->server.getLevelType() << uint8_t(m->server.getDefaultGameMode());
    login << uint8_t(m->server.getWorldType()) << uint8_t(m->server.getDifficulty());
    login << uint8_t(0) << uint8_t(m->server.getMaxPlayers());
    ss << login;
    m->player->loadData();

    m->server.getLogger() << "About to send world!\n";
    sendWorld();
    sendSpawnPosition();
}

void ClientConnection::sendWorld() {
    ChunkCoordinates playerChunk = m->player->getPosition();
    vector<ChunkCoordinates> loadingArea = playerChunk.getSurroundingChunks(3, true);
    World &world = m->player->getWorld();
    cout << "About to load all chunks!\n";
    vector<Chunk *> chunks = world.loadAll(loadingArea);
    cout << "Got chunks, about to send...\n";
    for (auto it=chunks.begin(); it != chunks.end(); ++it) {
        Packet pack;
        ChunkCoordinates coords = (*it)->getCoordinates();
        pack << PACKET_CHUNK_DATA << coords.x << coords.z;
        pack << uint8_t(true); // Ground-up continuous?
        pack << uint16_t(0xFFFF); // All 16 bits 1.
        pack << uint16_t(0xFFFF); // All 16 bits 1.

        uint8_t chunkData[(4096 + 2048 + 2048 + 2048 /*+ 2048 add bytes. */) * 16 + 256];
        cout << "about to copy chunk data\n";
        memcpy(chunkData, (*it)->blocks, 65536);
        cout << "About to memset\n";
        memset(&chunkData[65536], 0, sizeof(chunkData) - 65536);
        cout << "memset'd\n";
        ArrayOutputStream out(256);
        DeflateOutputStream dout(out);
        cout << "about to write chunk data\n";
        dout.write(chunkData, sizeof(chunkData));
        cout << "written chunk data\n";
        dout.close();
        pack << static_cast<int>(out.size());
        cout << "out.size() = " << out.size() << '\n';
        pack.add(out.data(), out.size());
        m->ss << pack;
        cout << "Sent chunk!\n";
    }
    cout << "Finished sending chunks!\n";
}

void ClientConnection::sendSpawnPosition() {
    Packet sp;
    sp << PACKET_SPAWN_POSITION;
    Point3D pos = m->player->getPosition();
    sp << int(pos.x) << int(pos.y) << int(pos.z);
    m->ss << sp;

    Packet pl;
    pl << PACKET_PLAYER_POSITION_AND_LOOK;
    pl << pos.x << double(0) << pos.y << pos.z;
    pl << float(0) << float(0) << uint8_t(1);
    m->ss << pl;

    cout << "Sent spawn position!\n";
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
    pack << PACKET_ENCRYPTION_KEY_RESPONSE << int(0); // 1 int = 2 shorts.

    m->ss << pack;

//    m->ess = EncryptedSocketStream(m->socketfd, &m->encryptor, &m->decryptor);
    m->ss = SocketStream(new EncryptedInputStream(m->socketfd, &m->decryptor), new EncryptedOutputStream(m->socketfd, &m->encryptor));
    m->encrypted = true;
//    setBlocking(false);
}

void ClientConnection::run() {
    m->server.getLogger() << "ClientConnection::run\n";
    init();
    // Read-handle loop here.
}


/**
 * Shuts down the connection, freeing any resources and removing any references to it.
 */
void ClientConnection::shutdown() {
    m->shutdown = true;
    close(m->socketfd);
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

