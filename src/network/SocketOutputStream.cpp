
#include <sstream>
#include <bitset>

#include <unistd.h>

#include "SocketOutputStream.hpp"
#include "NetUtils.hpp"
#include "../logging/Logger.hpp"
#include "../MinecraftServer.hpp"
#include "../Unicode.hpp"

using std::vector;

namespace MCServer {
namespace Network {

SocketOutputStream::SocketOutputStream(int socketfd)
    :socketfd(socketfd) {

}

SocketOutputStream & SocketOutputStream::operator<<(uint8_t data) {
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(int8_t data) {
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(uint16_t data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(int16_t data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(uint32_t data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(int32_t data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(uint64_t data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(int64_t data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(float data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(double data) {
    if (!bigEndian) {
        swapEndian(data);
    }
    write(socketfd, &data, sizeof(data));
    return *this;
}

SocketOutputStream & SocketOutputStream::operator<<(const std::string &data) {
    Logging::Logger &logger = MinecraftServer::getServer().getLogger();
    logger.info("operator<<: " + data);
    vector<uint16_t> usc2 = utf8ToUcs2(data);
    std::ostringstream oss;
    oss << "USC-2 data: ";
    for (auto it = usc2.begin(); it != usc2.end(); ++it) {
        uint16_t s = *it;
        oss /*<< ((char) (s << 8))*/ << (char) s;
    }
    logger.info(oss.str());
    std::ostringstream oss2;
    oss2 << "String length: " << data.size() << ", USC-2 length: " << usc2.size();
    logger.info(oss2.str());
    for (size_t i=0; i<usc2.size(); ++i) {
        logger << "usc2[" << i << "] = " << (char) usc2[i] << ", binary = " << std::bitset<16>(usc2[i]) << '\n';
        uint16_t u = usc2[i];
        usc2[i] = (u << 8) | (u >> 8);
    }
    operator<<(static_cast<uint16_t>(usc2.size()));
    write(socketfd, usc2.data(), usc2.size() * sizeof(uint16_t));
    return *this;
}

}
}
