
#include <string>
#include <iostream>

#include "../Unicode.hpp"
#include "../MinecraftServer.hpp"

using namespace MCServer;

int main() {
    MinecraftServer *server = new MinecraftServer;
    std::string s = "a";
    size_t pos = 0;
    uint32_t codePoint = utf8ToCodePoint(s, pos);
    std::cout << "codepoint of 'a' = " << codePoint;
    std::string out;
    codePointToUtf8(97, out);
    std::cout << "0x97 in unicode (expected 'a'): " << out;
}
