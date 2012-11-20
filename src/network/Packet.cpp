
#include <vector>
#include <iostream>

#include <Util/Endian.h>
#include <Util/Unicode.hpp>

#include "Packet.hpp"
#include "util/Utils.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"

using std::vector;
using std::cout;

using Util::ucs2ToUtf8;
using Util::utf8ToUcs2;

namespace MCServer {
namespace Network {


Packet::Packet()
:buffer(8, true) {

}

Packet::Packet(const vector<uint8_t> &v)
:buffer(v.size(), true) {
    buffer.add(v.data(), v.size());
}

Packet::Packet(const uint8_t *bytes, size_t length)
:buffer(length, true) {
/*    buffer.reserve(length);
    for (size_t i=0; i<length; ++i) {
        buffer.add(length);
    } */
    buffer.add(bytes, length);
}

Packet & Packet::operator<<(int8_t i) {
    buffer.addByte(i);
    return *this;
}

Packet & Packet::operator<<(uint8_t i) {
    buffer.addByte(i);
    return *this;
}

Packet & Packet::operator<<(int16_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
//    buffer.addByte(i);
//    buffer.addByte(i >> 8);
    buffer.add(&i, sizeof(i));
    return *this;
}

Packet & Packet::operator<<(uint16_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }

//    abcdefgh ijklmnop
//    buffer.addByte(i);
//    buffer.addByte(i >> 8);
    buffer.add(&i, sizeof(i));
    return *this;
}

Packet & Packet::operator<<(int32_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
/*    buffer.addByte(i);
    buffer.addByte(i >> 8);
    buffer.addByte(i >> 16);
    buffer.addByte(i >> 24); */
    buffer.add(&i, sizeof(i));
    return *this;
}

Packet & Packet::operator<<(uint32_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
/*    buffer.addByte(i);
    buffer.addByte(i >> 8);
    buffer.addByte(i >> 16);
    buffer.addByte(i >> 24); */
    buffer.add(&i, sizeof(i));
    return *this;
}

Packet & Packet::operator<<(int64_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
/*    buffer.addByte(i);
    buffer.addByte(i >> 8);
    buffer.addByte(i >> 16);
    buffer.addByte(i >> 24);
    buffer.addByte(i >> 32);
    buffer.addByte(i >> 40);
    buffer.addByte(i >> 48);
    buffer.addByte(i >> 56); */
    buffer.add(&i, sizeof(i));
    return *this;
}

Packet & Packet::operator<<(uint64_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
/*    buffer.addByte(i);
    buffer.addByte(i >> 8);
    buffer.addByte(i >> 16);
    buffer.addByte(i >> 24);
    buffer.addByte(i >> 32);
    buffer.addByte(i >> 40);
    buffer.addByte(i >> 48);
    buffer.addByte(i >> 56); */
    buffer.add(&i, sizeof(i));
    return *this;
}

Packet & Packet::operator<<(float f) {
    if (!bigEndian) {
        swapEndian(f);
    }
/*    buffer.addByte(floatToIntBits(f));
    buffer.addByte(floatToIntBits(f) >> 8);
    buffer.addByte(floatToIntBits(f) >> 16);
    buffer.addByte(floatToIntBits(f) >> 24); */
    buffer.add(&f, sizeof(f));
    return *this;
}

Packet & Packet::operator<<(double d) {
    // d is <native endian>
    if (!bigEndian) {
        swapEndian(d);
    }
    // d is big endian.
/*     Adds to buffer in current endian.
    1 2 3 4 5 6 7 8
    
    8 7 6 5 4 3 2 1
    buffer.addByte(doubleToInt64Bits(d));       8
    buffer.addByte(doubleToInt64Bits(d) >> 8);  7
    buffer.addByte(doubleToInt64Bits(d) >> 16); 6
    buffer.addByte(doubleToInt64Bits(d) >> 24); 5
    buffer.addByte(doubleToInt64Bits(d) >> 32); 4
    buffer.addByte(doubleToInt64Bits(d) >> 40); 3
    buffer.addByte(doubleToInt64Bits(d) >> 48); 2
    buffer.addByte(doubleToInt64Bits(d) >> 56); 1 */
    buffer.add(&d, sizeof(d));
    return *this;
}

Packet & Packet::operator<<(const std::string &s) {
    vector<uint16_t> ucs2 = utf8ToUcs2(s);
    operator<<(static_cast<uint16_t>(ucs2.size()));
    if (bigEndian) {
        for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
            buffer.addByte(*it);
            buffer.addByte(*it >> 8);
        }
    }
    else {
        for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
            buffer.addByte(*it >> 8);
            buffer.addByte(*it);
        }
    }
    return *this;
}

Packet & Packet::operator<<(PacketType packetType) {
    return operator<<(static_cast<uint8_t>(packetType));
}

Packet & Packet::operator>>(int8_t &i) {
    i = static_cast<int8_t>(buffer.take());
    return *this;
}

Packet & Packet::operator>>(uint8_t &i) {
    i = buffer.take();
    return *this;
}

Packet & Packet::operator>>(int16_t &i) {
    uint8_t b1 = buffer.take();
    uint8_t b2 = buffer.take();
    i = b1 << 8 | b2;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(uint16_t &i) {
    uint8_t b1 = buffer.take();
    uint8_t b2 = buffer.take();
    i = (b1 << 8) | b2;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(int32_t &i) {
    uint8_t b1 = buffer.take();
    uint8_t b2 = buffer.take();
    uint8_t b3 = buffer.take();
    uint8_t b4 = buffer.take();
    i = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(uint32_t &i) {
    uint8_t b1 = buffer.take();
    uint8_t b2 = buffer.take();
    uint8_t b3 = buffer.take();
    uint8_t b4 = buffer.take();
    i = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(int64_t &i) {
    uint64_t b1 = buffer.take();
    uint64_t b2 = buffer.take();
    uint64_t b3 = buffer.take();
    uint64_t b4 = buffer.take();
    uint64_t b5 = buffer.take();
    uint64_t b6 = buffer.take();
    uint64_t b7 = buffer.take();
    uint64_t b8 = buffer.take();
    i = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) | (b6 << 16) | (b7 << 8) | b8;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(uint64_t &i) {
    uint64_t b1 = buffer.take();
    uint64_t b2 = buffer.take();
    uint64_t b3 = buffer.take();
    uint64_t b4 = buffer.take();
    uint64_t b5 = buffer.take();
    uint64_t b6 = buffer.take();
    uint64_t b7 = buffer.take();
    uint64_t b8 = buffer.take();
    i = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) | (b6 << 16) | (b7 << 8) | b8;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(float &i) {
    uint8_t b1 = buffer.take();
    uint8_t b2 = buffer.take();
    uint8_t b3 = buffer.take();
    uint8_t b4 = buffer.take();
    i = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(double &i) {
    uint64_t b1 = buffer.take();
    uint64_t b2 = buffer.take();
    uint64_t b3 = buffer.take();
    uint64_t b4 = buffer.take();
    uint64_t b5 = buffer.take();
    uint64_t b6 = buffer.take();
    uint64_t b7 = buffer.take();
    uint64_t b8 = buffer.take();
    i = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) | (b6 << 16) | (b7 << 8) | b8;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(std::string &s) {
    uint16_t length;
    operator>>(length);
    vector<uint16_t> ucs2(length);
    buffer.take(ucs2.data(), length * 2);
    for (auto it=ucs2.begin(); it != ucs2.end(); ++it) {
        uint16_t u = *it;
        *it = (u << 8) | (u >> 8);
    }
    s = ucs2ToUtf8(ucs2);
    return *this;
}

Packet & Packet::operator>>(PacketType &p) {
    uint8_t b;
    operator>>(b);
    p = static_cast<PacketType>(b);
    return *this;
}

const uint8_t * Packet::getBytes() const {
    return buffer.begin();
}

size_t Packet::size() const {
    return buffer.available();
}

}
}
