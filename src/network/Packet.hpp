
#ifndef PACKET_HPP
#define PACKET_HPP

#include <vector>
#include <stdint.h>
#include <string>

#include "Packets.hpp"

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
    Packet & operator<<(Packets);

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
    Packet & operator>>(Packets &);

    template<class T>
    void add(T *data, size_t length) {
        buffer.reserve(length * sizeof(T));
        size_t i = 0;
        while (i < length) {
            buffer.push_back(data[i]);
            ++i;
        }
    }

    const uint8_t * getBytes() const;
    size_t size() const;
private:
    std::vector<uint8_t> buffer;
    size_t readPos;
};

}
}

#endif
