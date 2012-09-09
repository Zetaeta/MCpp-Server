
#ifndef PLAINSOCKETOUTPUTSTREAM_HPP
#define PLAINSOCKETOUTPUTSTREAM_HPP

#include <string>
#include <stdint.h>

#include "SocketOutputStream.hpp"

namespace MCServer {
namespace Network {

#ifndef PACKET_HPP
class Packet;
#endif

class PlainSocketOutputStream : public virtual SocketOutputStream {
public:
    PlainSocketOutputStream(int socketfd);
    PlainSocketOutputStream & operator<<(uint8_t);
    PlainSocketOutputStream & operator<<(int8_t);
    PlainSocketOutputStream & operator<<(uint16_t);
    PlainSocketOutputStream & operator<<(int16_t);
    PlainSocketOutputStream & operator<<(uint32_t);
    PlainSocketOutputStream & operator<<(int32_t);
    PlainSocketOutputStream & operator<<(uint64_t);
    PlainSocketOutputStream & operator<<(int64_t);
    PlainSocketOutputStream & operator<<(float);
    PlainSocketOutputStream & operator<<(double);
    PlainSocketOutputStream & operator<<(const std::string &);
    PlainSocketOutputStream & operator<<(const Packet &);

//    template<class T>
//    void write(const T *data, size_t length) {
//        write(socketfd, data, sizeof(T) * length);
//    }

    void writeRaw(const void *data, size_t length);
private:
    int socketfd;
};

}
}

#endif
