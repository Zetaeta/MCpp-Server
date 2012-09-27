
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>

#include "block/Block.hpp"

namespace MCServer {

typedef std::array<Block *, 16> BlocksZ;
typedef std::array<BlocksZ, 256> BlocksYZ;
typedef std::array<BlocksYZ, 16> BlocksXYZ, ChunkBlocks;

class Chunk {
    const BlocksYZ & operator[](uint8_t) const;
    BlocksYZ & operator[](uint8_t);
private:
    ChunkBlocks data;
};

}

#endif
