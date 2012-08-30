
#ifndef SOCKETOUTPUTSTREAM_HPP
#define SOCKETOUTPUTSTREAM_HPP

#include <string>
#include <stdint.h>

extern "C" ssize_t write(int fd, const void *buf, size_t count);

namespace MCServer {
namespace Network {

#ifndef PACKET_HPP
class Packet;
#endif

class SocketOutputStream {
public:
    SocketOutputStream(int socketfd);
    SocketOutputStream & operator<<(uint8_t);
    SocketOutputStream & operator<<(int8_t);
    SocketOutputStream & operator<<(uint16_t);
    SocketOutputStream & operator<<(int16_t);
    SocketOutputStream & operator<<(uint32_t);
    SocketOutputStream & operator<<(int32_t);
    SocketOutputStream & operator<<(uint64_t);
    SocketOutputStream & operator<<(int64_t);
    SocketOutputStream & operator<<(float);
    SocketOutputStream & operator<<(double);
    SocketOutputStream & operator<<(const std::string &);
    SocketOutputStream & operator<<(const Packet &);

    template<class T>
    void write(const T *data, size_t length) {
        write(socketfd, data, sizeof(T) * length);
    }

    void writeRaw(const void *data, size_t length);

private:
    int socketfd;
};

}
}

#endif
