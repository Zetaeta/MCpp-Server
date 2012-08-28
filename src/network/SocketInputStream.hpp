
#ifndef SOCKETINPUTSTREAM_HPP
#define SOCKETINPUTSTREAM_HPP

#include <string>
#include <stdint.h>

namespace MCServer {
namespace Network {

class SocketInputStream {
public:
    SocketInputStream(int socketfd);
    SocketInputStream & operator>>(uint8_t&);
    SocketInputStream & operator>>(int8_t&);
    SocketInputStream & operator>>(uint16_t&);
    SocketInputStream & operator>>(int16_t&);
    SocketInputStream & operator>>(uint32_t&);
    SocketInputStream & operator>>(int32_t&);
    SocketInputStream & operator>>(uint64_t&);
    SocketInputStream & operator>>(int64_t&);
    SocketInputStream & operator>>(std::string &);
    SocketInputStream & operator>>(float &);
    SocketInputStream & operator>>(double &);

    SocketInputStream & peek(uint8_t&);
    SocketInputStream & peek(int8_t&);
    SocketInputStream & peek(uint16_t&);
    SocketInputStream & peek(int16_t&);
    SocketInputStream & peek(uint32_t&);
    SocketInputStream & peek(int32_t&);
    SocketInputStream & peek(uint64_t&);
    SocketInputStream & peek(int64_t&);
    SocketInputStream & peek(std::string &);
    SocketInputStream & peek(float &);
    SocketInputStream & peek(double &);
private:
    int socketfd;
};

}
}

#endif
