
#include <unistd.h>

#include "SocketInputStream.hpp"
#include "NetUtils.hpp"

using std::vector;

namespace MCServer {
namespace Network {

SocketInputStream::SocketInputStream(int socketfd)
    :socketfd(socketfd) {

}

SocketInputStream & SocketInputStream::operator>>(uint8_t &data) {
    read(socketfd, &data, sizeof(data));
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
    vector<uint16_t> usc2(length);
    read(socketfd, usc2.data(), length * 2);
    data = usc2ToUtf8(usc2);
    return *this;
}

}
}

