
#ifndef SOCKETOUTPUTSTREAM_HPP
#define SOCKETOUTPUTSTREAM_HPP

#include <string>
#include <stdint.h>

// extern "C" ssize_t write(int fd, const void *buf, size_t count);

namespace MCServer {
namespace Network {

#ifndef PACKET_HPP
class Packet;
#endif

class SocketOutputStream {
public:
    virtual SocketOutputStream & operator<<(uint8_t) = 0;
    virtual SocketOutputStream & operator<<(int8_t) = 0;
    virtual SocketOutputStream & operator<<(uint16_t) = 0;
    virtual SocketOutputStream & operator<<(int16_t) = 0;
    virtual SocketOutputStream & operator<<(uint32_t) = 0;
    virtual SocketOutputStream & operator<<(int32_t) = 0;
    virtual SocketOutputStream & operator<<(uint64_t) = 0;
    virtual SocketOutputStream & operator<<(int64_t) = 0;
    virtual SocketOutputStream & operator<<(float) = 0;
    virtual SocketOutputStream & operator<<(double) = 0;
    virtual SocketOutputStream & operator<<(const std::string &) = 0;
    virtual SocketOutputStream & operator<<(const Packet &) = 0;

    template<class T>
    void write(const T *data, size_t length) {
        writeRaw(data, sizeof(T) * length);
    }

    virtual void writeRaw(const void *data, size_t length) = 0;
};

}
}

#endif
