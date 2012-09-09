
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>

#include "PlainSocketInputStream.hpp"
#include "NetUtils.hpp"
#include "../util/Unicode.hpp"
#include "../logging/Logger.hpp"
#include "../MinecraftServer.hpp"

using std::vector;

namespace MCServer {
namespace Network {

PlainSocketInputStream::PlainSocketInputStream(int socketfd)
    :socketfd(socketfd) {

}

PlainSocketInputStream & PlainSocketInputStream::operator>>(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    log << "PlainSocketInputStream::operator>>\n";
    ::read(socketfd, &data, sizeof(data));
    log << "Read data!\n";
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(int8_t &data) {
    ::read(socketfd, &data, sizeof(data));
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(uint16_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(int16_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(uint32_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(int32_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(uint64_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(int64_t &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(float &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(double &data) {
    ::read(socketfd, &data, sizeof(data));
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::operator>>(std::string &data) {
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

PlainSocketInputStream & PlainSocketInputStream::peek(uint8_t &data) {
    Logging::Logger &log = MinecraftServer::getServer().getLogger();
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(int8_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(uint16_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(int16_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(uint32_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(int32_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(uint64_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(int64_t &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(float &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(double &data) {
    recv(socketfd, &data, sizeof(data), MSG_PEEK);
    if (!bigEndian) {
        swapEndian(data);
    }
    return *this;
}

PlainSocketInputStream & PlainSocketInputStream::peek(std::string &data) {
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

void * PlainSocketInputStream::readRaw(void *data, size_t length) {
    ::read(socketfd, data, length);
    return data;
}

void * PlainSocketInputStream::readRaw(size_t length) {
    char * data = new char[length];
    ::read(socketfd, data, length);
    return data;
}

}
}

