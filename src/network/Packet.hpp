
#ifndef PACKET_HPP
#define PACKET_HPP

#include <vector>

namespace MCServer {
namespace Network {

class Packet {
public:
    Packet();
    Packet(const vector<uint8_t> &bytes);
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

    Packet & operator>>(int8_t);
    Packet & operator>>(uint8_t);
    Packet & operator>>(int16_t);
    Packet & operator>>(uint16_t);
    Packet & operator>>(int32_t);
    Packet & operator>>(uint32_t);
    Packet & operator>>(int64_t);
    Packet & operator>>(uint64_t);
    Packet & operator>>(float);
    Packet & operator>>(double);
    Packet & operator>>(const std::string &);
private:
    std::vector<uint8_t> buffer;
    size_t readPos;
}

}
}

#endif
