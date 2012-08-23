
#include <unistd.h>

#include "SocketInputStream.hpp"
#include "NetUtils.hpp"
#include "../Unicode.hpp"
#include "../logging/Logger.hpp"
#include "../MinecraftServer.hpp"

using std::vector;

namespace MCServer {
namespace Network {

SocketInputStream::SocketInputStream(int socketfd)
    :socketfd(socketfd) {

}

SocketInputStream & SocketInputStream::operator>>(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    log << "SocketInputStream::operator>>\n";
    read(socketfd, &data, sizeof(data));
    log << "Read data!\n";
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int8_t &data) {
    read(socketfd, &data, sizeof(data));
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(uint16_t &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int16_t &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(uint32_t &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int32_t &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(uint64_t &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int64_t &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(float &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(double &data) {
    read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(std::string &data) {
    uint16_t length;
    operator>>(length);
    vector<uint16_t> ucs2(length);
    read(socketfd, ucs2.data(), length * 2);
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    data = ucs2ToUtf8(ucs2);
    return *this;
}

}
}

