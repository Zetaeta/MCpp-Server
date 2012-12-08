
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <stdint.h>

namespace MCServer {

class Chunk;

class Block {
public:
    Block(Chunk &chunk, uint8_t x, uint8_t y, uint8_t z);
    virtual ~Block();

    uint16_t getId();
    uint8_t getMetadata();

private:
    Chunk &chunk;
    uint8_t x, y, z;
    uint8_t &id, &metadata;
};

}

#endif

