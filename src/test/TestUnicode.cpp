
#include <string>
#include <iostream>
#include <bitset>

#include "../Unicode.hpp"
#include "../MinecraftServer.hpp"

using namespace MCServer;

int main() {
    MinecraftServer *server = new MinecraftServer;
    std::string s = "§";
    size_t pos = 0;
    uint32_t codePoint = utf8ToCodePoint(s, pos);
    std::cout << "s.size() = " << s.size() << '\n';
    std::cout << std::hex << "€[0] = " << std::bitset<8>(s[0]) << ", €[1] = " << std::bitset<8>(s[1]) << ", €[2] = " << std::bitset<8>(s[2]) << '\n'; 
    std::cout << "codepoint of '€' = 0x" << std::hex << codePoint << '\n';
    std::string out;
    codePointToUtf8(codePoint, out);
    std::cout << "out.size(): " << out.size() << '\n';
    std::cout << "0x" << std::hex << codePoint << " in unicode (expected '€'): " << out << '\n';
}

