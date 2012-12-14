
#include <iostream>

#include "Block.hpp"
#include "Chunk.hpp"

namespace MCServer {

Block::Block(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z)
:chunk(&chunk), x(x), y(y), z(z), id(&chunk.blockIds[index(x, y, z)]), metadata(&chunk.blockMetadata[index(x, y, z)]) {
    std::cout << "x = " << uint16_t(x) << ", y = " << uint16_t(y) << ", z = " << uint16_t(z) << ", id = " << static_cast<void *>(id) << ", chunk = " << &chunk << '\n';
}

Block::Block()
:chunk(nullptr), id(nullptr), metadata(nullptr) {}


Block::~Block() {

}

uint16_t Block::getId() const {
    std::cout << "id = " << static_cast<void *>(id) << '\n';
    return *id;
}

uint8_t Block::getMetadata() const {
    return *metadata;
}

BlockCoordinates Block::getPosition() const {
    ChunkCoordinates cc = chunk->getCoordinates();
    return BlockCoordinates(cc.x * 16 + x, y, cc.z * 16 + z);
}

Block::operator bool() const {
    return id != nullptr;
}

}

