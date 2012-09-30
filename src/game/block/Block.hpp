
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <stdint.h>

namespace MCServer {

class Block {
public:
    Block(uint8_t = 0, uint8_t = 0);
    virtual ~Block();

    uint16_t getId();
    uint8_t getMetadata();

    static Block * newBlock(uint8_t id, uint8_t metadata);
//private:
    uint8_t id;
    uint8_t metadata;
};

}

#endif

