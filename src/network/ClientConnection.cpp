
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <IOStream/ArrayOutputStream.hpp>
#include <IOStream/DeflateOutputStream.hpp>
#include <IOStream/ArrayInputStream.hpp>
#include <IOStream/DeflateInputStream.hpp>
#include <IOStream/SocketStream.hpp>
#include <IOStream/EncryptedInputStream.hpp>
#include <IOStream/EncryptedOutputStream.hpp>

#include "ClientConnection.hpp"
#include "logging/Logger.hpp"
#include "MinecraftServer.hpp"
#include "PacketHandler.hpp"
#include "PacketType.hpp"
#include "Scheduler.hpp"
#include "ChatServer.hpp"

#include "game/entity/Player.hpp"
#include "game/ChunkCoordinates.hpp"
#include "game/World.hpp"
#include "game/Chunk.hpp"
#include "game/Point3D.hpp"
#include "game/EntityManager.hpp"

using std::string;
using std::vector;
using std::cout;
using std::make_shared;
using std::shared_ptr;
using std::for_each;

using IOStream::ArrayOutputStream;
using IOStream::DeflateOutputStream;
using IOStream::ArrayInputStream;
using IOStream::DeflateInputStream;
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
    shared_ptr<Player> player;
    int socketfd;
    uint8_t protocolVersion;
    ChunkCoordinates sentWorldCentre;
    
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
//    pthread_create(&m->threadId, NULL, &startClientConnection, this);
    MinecraftServer::server().getScheduler().startThread(&ClientConnection::run, this);
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
    log << "About to read packet\n";
    
    ss >> m->protocolVersion >> username >> serverHost >> serverPort;
    log << "Read packet!\n";
//    m->player = new Player(username, *this);
    m->player = make_shared<Player>(username, *this);
//    m->player = player.get();
    m->server.getLogger() << "User " << username << " attempting to log into " << serverHost << ":" << serverPort << " with protocol version " << static_cast<int>(m->protocolVersion) << '\n';

    m->server.getLogger() << "About to send encryption request!\n";
    Packet &p = PacketHandler::encryptionRequest();
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
//    m->server.getLogger() << INFO << "Decrypted shared secret from user " << username << ": " << string(reinterpret_cast<const char *>(buffer), secretLen) << '\n';

    setupCrypto(string(reinterpret_cast<const char *>(buffer), secretLen));
    finishLogin();
}

void ClientConnection::run() {
    m->server.getLogger() << "ClientConnection::run\n";
    init();

    while (!m->shutdown) {
        PacketType packetType;
        m->ss >> packetType;
        handlePacket(packetType);
    }
    // Read-handle loop here.
}

/**
 * Shuts down the connection, freeing any resources and removing any references to it.
 */
void ClientConnection::shutdown() {
    cout << "ClientConnection::shutdown()\n";
    MinecraftServer::server().removePlayer(m->player);
//    m->player->getWorld().removeEntity(m->player);
    m->shutdown = true;
    close(m->socketfd);
}

void ClientConnection::finishLogin() {
    Logger &log = MinecraftServer::server().getLogger();

    SocketStream &ss = m->ss;
    uint8_t clientStatusId;
    uint8_t payload;
    clientStatusId = ss.readUByte();
    if (clientStatusId != PACKET_CLIENT_STATUS) {
        log << WARNING << "Invalid client status header: 0x" << std::hex << static_cast<uint16_t>(clientStatusId) << '\n' << std::dec;
//        shutdown();
//        return;
//        if (clientStatusId == 0) {
//            goto meow;
//        }
    }
    cout << "Read client status header\n";
    ss >> payload;
    if (payload) {
        log << WARNING << "Invalid client status payload: 0x" << std::hex << static_cast<uint16_t>(payload) << '\n' << std::dec;
        shutdown();
        return;
    }
    cout << "Read client status\n";
    log << INFO << "Read client status packet!\n";

    Packet login;
    login << PACKET_LOGIN_REQUEST;
    login << m->player->getId();
    login << m->server.getLevelType() << uint8_t(m->server.getDefaultGameMode());
    login << uint8_t(m->server.getWorldType()) << uint8_t(m->server.getDifficulty());
    login << uint8_t(0) << uint8_t(m->server.getMaxPlayers());
    ss << login;
    m->player->loadData();
//    m->player->getWorld().addEntity(m->player);
    MinecraftServer::server().addPlayer(m->player);

    m->server.getLogger() << "About to send world!\n";
    sendWorld();
    sendSpawnPosition();
    return;

    Packet p;
    p << PACKET_CHAT_MESSAGE;
    p << "Hello World!";
    ss << p;
    Packet p2;
    p2 << PACKET_CHAT_MESSAGE;
    p2 << ("*Hugs " + m->player->getName() + "*");
    ss << p2;
}

void ClientConnection::sendWorld() {
    ChunkCoordinates playerChunk = m->player->getPosition();
    vector<ChunkCoordinates> loadingArea = playerChunk.getSurroundingChunks(3, true);
    World &world = m->player->getWorld();
    cout << "About to load all chunks!\n";
    vector<shared_ptr<Chunk>> chunks = world.loadAll(loadingArea);
    cout << "Got chunks, about to send...\n";
    for (auto it=chunks.begin(); it != chunks.end(); ++it) {
        sendChunk(**it);
/*        Packet pack;
        ChunkCoordinates coords = (*it)->getCoordinates();
        pack << PACKET_CHUNK_DATA << coords.x << coords.z;
        pack << uint8_t(true); // Ground-up continuous?
        pack << uint16_t(0xFFFF); // All 16 bits 1.
        pack << uint16_t(0x0); // All 16 bits 1.

        uint8_t chunkData[(4096 + 2048 + 2048 + 2048 / *+ 2048 add bytes. * /) * 16 + 256];
        memset(chunkData, 0, sizeof(chunkData));
        for (uint32_t i = 0; i<65536; ++i) {
            chunkData[i] = (*it)->blocks[i].id;
        }
        for (uint32_t i=65536 + 65536 / 2; i<(65536 * 2 + 65536 / 2); ++i) {
            chunkData[i] = 0xFF;
        }
        for (uint32_t i = sizeof(chunkData) - 256; i<sizeof(chunkData); ++i) {
            chunkData[i] = 6;
        }

        ArrayOutputStream out(512);
        DeflateOutputStream dout(out);
        dout.write(chunkData, sizeof(chunkData));
        dout.close();
        uint32_t size = out.size();
        pack << size;
        pack.add(out.data(), size);
        ArrayInputStream in(out.data(), size);
        DeflateInputStream din(in);
        uint8_t meow;
        size_t blockLen = 0;
        while (din.read(&meow, 1) == 1) {
            ++blockLen;
        }
        
        m->ss << pack;
        cout << "Sent chunk " << coords << "!\n";
        */
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
    pl << pos.x << pos.y << pos.y << pos.z;
    pl << float(0) << float(0) << uint8_t(1);
    m->ss << pl;

    cout << "Sent spawn position!\n";
    cout << "pos = " << pos << '\n';

//    uint8_t header = m->ss.readUByte();
//    cout << "header = 0x" << std::hex << uint16_t(header) << std::dec << '\n';
//    shutdown();
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

void ClientConnection::sendChunk(const Chunk &ch) {
    Packet pack;
    ChunkCoordinates coords = ch.getCoordinates();
    pack << PACKET_CHUNK_DATA << coords.x << coords.z;
    pack << uint8_t(true); // Ground-up continuous?
    pack << uint16_t(0xFFFF); // All 16 bits 1.
    pack << uint16_t(0x0); // All 16 bits 1.

    uint8_t chunkData[(4096 + 2048 + 2048 + 2048 /*+ 2048 add bytes. */) * 16 + 256];
    memset(chunkData, 0, sizeof(chunkData));

    memcpy(chunkData, ch.blockIds, 65536);

    uint8_t *chunkMetaData = chunkData + 65536;
    memcpy(chunkMetaData, ch.blockMetadata, 65536 / 2);

    for (uint32_t i=65536 + 65536 / 2; i<(65536 * 2 + 65536 / 2); ++i) {
        chunkData[i] = 0xFF; // TODO: Implement proper extra block info stuff.
    }
    for (uint32_t i = sizeof(chunkData) - 256; i<sizeof(chunkData); ++i) {
        chunkData[i] = 6; // TODO: Implement biomes
    }

    ArrayOutputStream out(512);
    DeflateOutputStream dout(out);
    dout.write(chunkData, sizeof(chunkData));
    dout.close();
    uint32_t size = out.size();
    pack << size;
    pack.add(out.data(), size);
    
    m->ss << pack;
}

void ClientConnection::handlePacket(PacketType type) {
    static bool meow = false;
    if (meow) return;
    cout << "ClientConnection::handlePacket(0x" << std::hex << uint16_t(type) << std::dec << ")\n";
    switch (type) {
    case PACKET_KEEP_ALIVE: {
        int keepAliveId = m->ss.readInt();
        break;
    }
    case PACKET_DISCONNECT_KICK: {
//        m->ss.readString();
        shutdown();
        break;
    }
    case PACKET_PLAYER_POSITION_AND_LOOK: {
        receivePlayerPositionAndLook();
        break;
    }
    case PACKET_CLIENT_SETTINGS:
        receiveClientSettings();
        break;
    case PACKET_PLAYER_POSITION:
        receivePlayerPosition();
        break;
    case PACKET_PLAYER_ABILITIES:
        receivePlayerAbilities();
        break;
    case PACKET_PLAYER_LOOK:
        receivePlayerLook();
        break;
    case PACKET_CHAT_MESSAGE:
        receiveChatMessage();
        break;
    case PACKET_ANIMATION:
        receiveAnimation();
        break;
    case PACKET_PLAYER_DIGGING:
        receivePlayerDigging();
        break;
    default:
        cout << "default\n";
        meow = true;
//        shutdown();
        return;
    }
}

void ClientConnection::receiveClientSettings() {
    cout << "Player " << m->player->getName() << " has settings:\n";
    cout << "Locale: " << m->ss.readString() << '\n';
    cout << "View distance: " << uint16_t(m->ss.readUByte()) << '\n';
    cout << "Chat flags: " << uint16_t(m->ss.readUByte()) << '\n';
    cout << "Difficulty: " << uint16_t(m->ss.readUByte()) << '\n';
    if (m->protocolVersion >= 40) {
        cout << "Show cape: " << uint16_t(m->ss.readUByte()) << '\n';
    }
}

void ClientConnection::receivePlayerPositionAndLook() {
    Point3D pos;
    m->ss >> pos.x >> pos.y;
    double stance = m->ss.readDouble();
    m->ss >> pos.z;
    float yaw = m->ss.readFloat();
    float pitch = m->ss.readFloat();
    m->ss.readByte();

    m->player->setPosition(pos);
    updatePosition();
}

void ClientConnection::receivePlayerPosition() {
    Point3D pos;
    m->ss >> pos.x;
    m->ss >> pos.y;
    double stance = m->ss.readDouble();
    m->ss >> pos.z;
    m->player->setPosition(pos);
    m->ss.readByte();
    updatePosition();
}

void ClientConnection::receivePlayerAbilities() {
    uint8_t flags = m->ss.readUByte();
    uint8_t flySpeed = m->ss.readUByte();
    uint8_t walkSpeed = m->ss.readUByte();
    cout << "flags = 0x" << std::hex << uint16_t(flags) << std::dec << '\n';
    cout << "flySpeed = " << uint16_t(flySpeed) << '\n';
    cout << "walkSpeed = " << uint16_t(walkSpeed) << '\n';
}

void ClientConnection::receivePlayerLook() {
    m->player->setYaw(m->ss.readFloat());
    m->player->setPitch(m->ss.readFloat());
    cout << "onGround: " << uint16_t(m->ss.readByte()) << '\n';
}

void ClientConnection::receiveChatMessage() {
    string message = m->ss.readString();
    MinecraftServer::server().getChatServer().handleChatMessage(message, m->player);
}

void ClientConnection::receiveAnimation() {
    int eid = m->ss.readInt();
    uint8_t animation = m->ss.readUByte();
    cout << "receiveAnimation: eid = " << eid << ", animation = " << uint16_t(animation) << '\n';
    MinecraftServer::server().getEntityManager().sendAnimation(eid, animation);
}

void ClientConnection::receivePlayerDigging() {
    uint8_t status = m->ss.readUByte();
    int x = m->ss.readInt();
    uint8_t y = m->ss.readUByte();
    int z = m->ss.readInt();
    uint8_t face = m->ss.readUByte();

    switch (status) {
    case 0: // Start digging
        m->player->setDigging(x, y, z, face);
        break;
    case 1:
        m->player->setDigging(false);
        break;
    case 2:
        m->player->setDigging(false);
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
}

void ClientConnection::updatePosition() {
    ChunkCoordinates pos = m->player->getPosition();
    if (pos != m->sentWorldCentre) {
        vector<ChunkCoordinates> loadingArea = pos.getSurroundingChunks(3, true);
        World &world = m->player->getWorld();
        vector<shared_ptr<Chunk>> chunks = world.loadAll(loadingArea);
        for_each(chunks.begin(), chunks.end(), [&] (const shared_ptr<Chunk> ch) {
            sendChunk(*ch);
        });
    }
    m->sentWorldCentre = pos;
}

void ClientConnection::sendKeepAlive() {
    Packet ka;
    ka << PACKET_KEEP_ALIVE << int(0);
    m->ss << ka;
}

void ClientConnection::sendMessage(const std::string &message) {
    Packet pk;
    pk << PACKET_CHAT_MESSAGE << message;
    m->ss << pk;
}

}
}

