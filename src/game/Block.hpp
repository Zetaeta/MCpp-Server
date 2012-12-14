
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <stdint.h>

#include "BlockCoordinates.hpp"

namespace MCServer {

class Chunk;

class Block {
public:
    Block();
    Block(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z);
    virtual ~Block();

    uint16_t getId() const;
    uint8_t getMetadata() const;

    BlockCoordinates getPosition() const;

    operator bool() const;

private:
    Chunk *chunk;
    int x, y, z;
    uint8_t *id, *metadata;
};

}

#endif

