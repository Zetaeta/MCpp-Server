
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>

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
    ::read(socketfd, &data, sizeof(data));
    log << "Read data!\n";
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int8_t &data) {
    ::read(socketfd, &data, sizeof(data));
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(uint16_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int16_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(uint32_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int32_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(uint64_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(int64_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(float &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(double &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::operator>>(std::string &data) {
    uint16_t length;
    operator>>(length);
    vector<uint16_t> ucs2(length);
    ::read(socketfd, ucs2.data(), length * 2);
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    data = ucs2ToUtf8(ucs2);
    return *this;
}

SocketInputStream & SocketInputStream::peek(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    return *this;
}

SocketInputStream & SocketInputStream::peek(int8_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    return *this;
}

SocketInputStream & SocketInputStream::peek(uint16_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(int16_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(uint32_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(int32_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(uint64_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(int64_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(float &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(double &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

SocketInputStream & SocketInputStream::peek(std::string &data) {
    uint16_t length;
    peek(length);
    vector<uint16_t> ucs2(length);
    uint16_t *rawdata = new uint16_t[length + 1];
    recv(socketfd, rawdata, (length * 2) + 2, MSG_PEEK);
    memcpy(ucs2.data(), rawdata + 1, length * 2);
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    data = ucs2ToUtf8(ucs2);
    return *this;
}

void * SocketInputStream::readRaw(void *data, size_t length) {
    ::read(socketfd, data, length);
    return data;
}

void * SocketInputStream::readRaw(size_t length) {
    char * data = new char[length];
    ::read(socketfd, data, length);
    return data;
}

}
}

