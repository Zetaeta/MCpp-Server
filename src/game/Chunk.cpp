
#include "Chunk.hpp"

namespace MCServer {

const BlocksYZ & Chunk::operator[](uint8_t x) const {
    return data[x];
}

BlocksYZ & Chunk::operator[](uint8_t x) {
    return data[x];
}

}
