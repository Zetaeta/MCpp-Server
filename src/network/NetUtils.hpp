
#ifndef NETUTILS_HPP
#define NETUTILS_HPP

#include <stdint.h>

namespace MCServer {
namespace Network {

bool isBigEndian();

extern const bool bigEndian;

void swapEndian(uint16_t &);

void swapEndian(int16_t &);

void swapEndian(uint32_t &);

void swapEndian(int32_t &);

void swapEndian(uint64_t &);

void swapEndian(int64_t &);

void swapEndian(float &);

void swapEndian(double &);

bool isBlocking(int socketfd);

void setBlocking(int socketfd, bool blocking);

}
}

#endif
