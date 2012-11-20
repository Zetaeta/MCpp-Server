
#ifndef PACKET_HPP
#define PACKET_HPP

#include <stdint.h>
#include <iostream>

#include <IOStream/Buffer.hpp>

#include <Util/stlfwd.hpp>
#include "PacketType.hpp"

namespace MCServer {
namespace Network {

class Packet {
public:
    Packet();
    Packet(const std::vector<uint8_t> &bytes);
    Packet(const uint8_t *bytes, size_t length);

    Packet & operator<<(int8_t);
    Packet & operator<<(uint8_t);
    Packet & operator<<(int16_t);
    Packet & operator<<(uint16_t);
    Packet & operator<<(int32_t);
    Packet & operator<<(uint32_t);
    Packet & operator<<(int64_t);
    Packet & operator<<(uint64_t);
    Packet & operator<<(float);
    Packet & operator<<(double);
    Packet & operator<<(const std::string &);
    Packet & operator<<(PacketType);

    Packet & operator>>(int8_t &);
    Packet & operator>>(uint8_t &);
    Packet & operator>>(int16_t &);
    Packet & operator>>(uint16_t &);
    Packet & operator>>(int32_t &);
    Packet & operator>>(uint32_t &);
    Packet & operator>>(int64_t &);
    Packet & operator>>(uint64_t &);
    Packet & operator>>(float &);
    Packet & operator>>(double &);
    Packet & operator>>(std::string &);
    Packet & operator>>(PacketType &);

    template<class T>
    void add(T *data, size_t length) {
        buffer.add(data, sizeof(T) * length);
    }

    const uint8_t * getBytes() const;
    size_t size() const;
private:
    IOStream::Buffer buffer;
};

template<class T>
T & operator<<(T &stream, const Packet &p) {
//    std::cout << "p.size() = " << p.size() << '\n';
    stream.write(p.getBytes(), p.size());
    return stream;
}

}
}

#endif
