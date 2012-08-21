
#ifndef SOCKETOUTPUTSTREAM_HPP
#define SOCKETOUTPUTSTREAM_HPP

#include <string>

namespace MCServer {
namespace Network {

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
private:
    int socketfd;
};

}
}

#endif
