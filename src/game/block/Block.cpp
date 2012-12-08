
#include "Block.hpp"

namespace MCServer {

Block::Block(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z)
:chunk(chunk), x(x), y(y), z(z), id(chunk.blockIds[index(x, y, z)]), metadata(chunk.blockMetadata[index(x, y, z)]) {
    
}

Block::~Block() {

}

uint16_t Block::getId() {
    return id;
}

uint8_t Block::getMetadata() {
    return metadata;
}

}

