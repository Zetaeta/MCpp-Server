
#include "Packet.hpp"
#include "NetUtils.hpp"

namespace MCServer {
namespace Network {

Packet::Packet()
:readPos(0) {

}

Packet::Packet(const vector<uint8_t> &v)
:buffer(v) {

}

Packet::Packet(const uint8_t *bytes, size_t length) {
    buffer.reserve(length);
    for (size_t i=0; i<length; ++i) {
        buffer.push_back(length);
    }
}

Packet & Packet::operator<<(int8_t i) {
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(uint8_t i) {
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(int16_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(uint16_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(int32_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
    buffer.push_back(i >> 24);
    buffer.push_back(i >> 16);
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(uint32_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
    buffer.push_back(i >> 24);
    buffer.push_back(i >> 16);
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(int64_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
    buffer.push_back(i >> 56);
    buffer.push_back(i >> 48);
    buffer.push_back(i >> 40);
    buffer.push_back(i >> 32);
    buffer.push_back(i >> 24);
    buffer.push_back(i >> 16);
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(uint64_t i) {
    if (!bigEndian) {
        swapEndian(i);
    }
    buffer.push_back(i >> 56);
    buffer.push_back(i >> 48);
    buffer.push_back(i >> 40);
    buffer.push_back(i >> 32);
    buffer.push_back(i >> 24);
    buffer.push_back(i >> 16);
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(float f) {
    if (!bigEndian) {
        swapEndian(f);
    }
    buffer.push_back(i >> 24);
    buffer.push_back(i >> 16);
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(double d) {
    if (!bigEndian) {
        swapEndian(d);
    }
    buffer.push_back(i >> 56);
    buffer.push_back(i >> 48);
    buffer.push_back(i >> 40);
    buffer.push_back(i >> 32);
    buffer.push_back(i >> 24);
    buffer.push_back(i >> 16);
    buffer.push_back(i >> 8);
    buffer.push_back(i);
    return *this;
}

Packet & Packet::operator<<(const std::string &s) {
    vector<uint16_t> ucs2 = utf8ToUcs2(s);
    operator<<(static_cast<uint16_t>(ucs2.size()));
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        buffer.push_back(*it);
        buffer.push_back(*it >> 8);
    }
    return *this;
}

Packet & Packet::operator>>(int8_t &i) {
    i = static_cast<int8_t>(buffer[readPos++]);
    return *this;
}

Packet & Packet::operator>>(uint8_t &i) {
    i = buffer[readPos++];
    return *this;
}

Packet & Packet::operator>>(int16_t &i) {
    uint8_t b1 = buffer[readPos++];
    uint8_t b2 = buffer[readPos++];
    i = b1 << 8 | b2;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(uint16_t &i) {
    uint8_t b1 = buffer[readPos++];
    uint8_t b2 = buffer[readPos++];
    i = (b1 << 8) | b2;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(int32_t &i) {
    uint8_t b1 = buffer[readPos++];
    uint8_t b2 = buffer[readPos++];
    uint8_t b3 = buffer[readPos++];
    uint8_t b4 = buffer[readPos++];
    i = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(uint32_t &i) {
    uint8_t b1 = buffer[readPos++];
    uint8_t b2 = buffer[readPos++];
    uint8_t b3 = buffer[readPos++];
    uint8_t b4 = buffer[readPos++];
    i = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(int64_t &i) {
    uint64_t b1 = buffer[readPos++];
    uint64_t b2 = buffer[readPos++];
    uint64_t b3 = buffer[readPos++];
    uint64_t b4 = buffer[readPos++];
    uint64_t b5 = buffer[readPos++];
    uint64_t b6 = buffer[readPos++];
    uint64_t b7 = buffer[readPos++];
    uint64_t b8 = buffer[readPos++];
    i = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) | (b6 << 16) | (b7 << 8) | b8;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(uint64_t &i) {
    uint64_t b1 = buffer[readPos++];
    uint64_t b2 = buffer[readPos++];
    uint64_t b3 = buffer[readPos++];
    uint64_t b4 = buffer[readPos++];
    uint64_t b5 = buffer[readPos++];
    uint64_t b6 = buffer[readPos++];
    uint64_t b7 = buffer[readPos++];
    uint64_t b8 = buffer[readPos++];
    i = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) | (b6 << 16) | (b7 << 8) | b8;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(float &i) {
    uint8_t b1 = buffer[readPos++];
    uint8_t b2 = buffer[readPos++];
    uint8_t b3 = buffer[readPos++];
    uint8_t b4 = buffer[readPos++];
    i = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(double &i) {
    uint64_t b1 = buffer[readPos++];
    uint64_t b2 = buffer[readPos++];
    uint64_t b3 = buffer[readPos++];
    uint64_t b4 = buffer[readPos++];
    uint64_t b5 = buffer[readPos++];
    uint64_t b6 = buffer[readPos++];
    uint64_t b7 = buffer[readPos++];
    uint64_t b8 = buffer[readPos++];
    i = (b1 << 56) | (b2 << 48) | (b3 << 40) | (b4 << 32) | (b5 << 24) | (b6 << 16) | (b7 << 8) | b8;
    if (!bigEndian) {
        swapEndian(i);
    }
    return *this;
}

Packet & Packet::operator>>(std::string &s) {
    uint16_t length;
    operator>>(length);
//    vector<uint16_t> ucs2 = utf8ToUcs2(s);
//    operator<<(static_cast<uint16_t>(ucs2.size()));
    for (auto it = ucs2.begin(); it != ucs2.end(); ++it) {
        buffer.push_back(*it);
        buffer.push_back(*it >> 8);
    }
    return *this;
}

}
}
