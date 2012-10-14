
#include <iostream>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include "NetUtils.hpp"

using std::string;

namespace MCServer {
namespace Network {


bool isBigEndian() {
    unsigned char endianTest[] = {1, 0};
    short *x = reinterpret_cast<short *>(endianTest);
    if (*x == 256) {
        return true;
    }
    else if (*x == 1) {
        return false;
    }
    else {
        std::cout << "Error: invalid endian: x = " << x << ", must be 256 or 1";
        exit(1);
    }
}

extern const bool bigEndian = isBigEndian();

void swapEndian(uint16_t &data) {
    uint8_t b1, b2;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    data = (b1 << 8) | b2;
}

void swapEndian(int16_t &data) {
    uint8_t b1, b2;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    data = (b1 << 8) | b2;
}

void swapEndian(uint32_t &data) {
    uint8_t b1, b2, b3, b4;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    b3 = (data >> 16) & 0xFF;
   b4 = (data >> 24) & 0xFF;
    data = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

void swapEndian(int32_t &data) {
    uint8_t b1, b2, b3, b4;
    b1 = data & 0xFF;
    b2 = (data >> 8) & 0xFF;
    b3 = (data >> 16) & 0xFF;
    b4 = (data >> 24) & 0xFF;
    data = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

void swapEndian(uint64_t &data) {
    uint64_t b[8];
    b[0] = data & 0xFF;
    b[1] = (data >> 8) & 0xFF;
    b[2] = (data >> 16) & 0xFF;
    b[3] = (data >> 24) & 0xFF;
    b[4] = (data >> 32) & 0xFF;
    b[5] = (data >> 40) & 0xFF;
    b[6] = (data >> 48) & 0xFF;
    b[7] = (data >> 56) & 0xFF; 
    data = (b[0] << 56) | (b[1] << 48) | (b[2] << 40) | (b[3] << 32) | (b[4] << 24) | (b[5] << 16) | (b[6] << 8) | b[7];
}

void swapEndian(int64_t &data) {
    uint64_t b[8];
    b[0] = data & 0xFF;
    b[1] = (data >> 8) & 0xFF;
    b[2] = (data >> 16) & 0xFF;
    b[3] = (data >> 24) & 0xFF;
    b[4] = (data >> 32) & 0xFF;
    b[5] = (data >> 40) & 0xFF;
    b[6] = (data >> 48) & 0xFF;
    b[7] = (data >> 56) & 0xFF; 
    data = (b[0] << 56) | (b[1] << 48) | (b[2] << 40) | (b[3] << 32) | (b[4] << 24) | (b[5] << 16) | (b[6] << 8) | b[7];
}

void swapEndian(float &data) {
    union {
        float f;
        unsigned char b[4];
    } dat1, dat2;
    dat1.f = data;
    dat2.b[0] = dat1.b[3];
    dat2.b[1] = dat1.b[2];
    dat2.b[2] = dat1.b[1];
    dat2.b[3] = dat1.b[0];
    data = dat2.f;
}

void swapEndian(double &data) {
    union {
        double d;
        unsigned char b[8];
    } dat1, dat2;
    dat1.d = data;
    dat2.b[0] = dat1.b[7];
    dat2.b[1] = dat1.b[6];
    dat2.b[2] = dat1.b[5];
    dat2.b[3] = dat1.b[4];
    dat2.b[4] = dat1.b[3];
    dat2.b[5] = dat1.b[2];
    dat2.b[6] = dat1.b[1];
    dat2.b[7] = dat1.b[0];
    data = dat2.d;
}

bool isBlocking(int socketfd) {
    return !(fcntl(socketfd, F_GETFL) & O_NONBLOCK);
}

void setBlocking(int socketfd, bool blocking) {
    int fl = fcntl(socketfd, F_GETFL);
    if (blocking) {
        fl &= ~O_NONBLOCK;
    }
    else {
        fl |= O_NONBLOCK;
    }
    fcntl(socketfd, F_SETFL, fl);
}


}
}
