
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <array>

#include "Block.hpp"

namespace MCServer {

typedef std::array<Block, 16> BlocksZ;
typedef std::array<BlockZ, 256> BlocksYZ;
typedef std::array<BlockYZ, 16> BlocksXYZ, ChunkBlocks;

class Chunk {
    const BlocksYZ & operator[](uint8_t) const;
    BlocksYZ & operator[]();
private:
    ChunkBlocks data;
};

}

#endif
