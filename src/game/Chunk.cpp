
#include "Chunk.hpp"

namespace MCServer {

const BlocksYZ & operator[](uint8_t x) const {
    return data[x];
}

BlocksYZ & operator[](uint8_t x) {
    return data[x];
}

}
