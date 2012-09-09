
#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <vector>
#include <string>
#include <stdint.h>

namespace MCServer {

std::vector<uint16_t> utf8ToUcs2(const std::string &);
std::string ucs2ToUtf8(const std::vector<uint16_t> &);

uint32_t utf8ToCodePoint(const std::string &utf8, size_t &position);
//uint32_t ucs2ToCodePoint(const std::vector<uint16_t> &ucs2, size_t &position);

//void codePointToUcs2(uint32_t codePoint, std::vector<uint16_t> &ucs2);
void codePointToUtf8(uint32_t codePoint, std::string &utf8);

}

#endif
