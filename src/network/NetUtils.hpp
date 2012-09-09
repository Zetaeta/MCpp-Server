
#ifndef NETUTILS_HPP
#define NETUTILS_HPP

#include <stdint.h>
#include <vector>
#include <string>

namespace MCServer {
namespace Network {

//uint32_t codePointFromUtf8(const std::string &string, size_t &pos);

//std::vector<uint16_t> utf8ToUsc2(const std::string &string);

//std::string usc2ToUtf8(const std::vector<uint16_t> &);

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
