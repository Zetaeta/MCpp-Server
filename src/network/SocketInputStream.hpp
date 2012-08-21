
#ifndef SOCKETINPUTSTREAM_HPP
#define SOCKETINPUTSTREAM_HPP

#include <string>

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
private:
    int socketfd;
};

}
}

#endif
