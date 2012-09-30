
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block/Block.hpp"
#include "util/ArrayAccessor.hpp"

namespace NBT {
class TagCompound;
}

namespace MCServer {

typedef ArrayAccessor<Block, 16, 16, 256> ChunkData, BlocksXZY, Blocks3D;
typedef ArrayAccessor<Block, 16, 256> BlocksZY, Blocks2D;
typedef ArrayAccessor<Block, 256> BlocksY, Blocks1D;

class Chunk {
public:
    Chunk();
    Chunk(const Chunk &);
//    const BlocksYZ & operator[](uint8_t) const;
//    BlocksYZ & operator[](uint8_t);
    ArrayAccessor<Block, 16, 256> operator[](size_t index) {
        return blocksAccess[index];
    }

    ArrayAccessor<const Block, 16, 256> operator[](size_t index) const {
        return blocksAccess[index];
    }

    void loadFrom(const NBT::TagCompound &compound);
    void loadSection(const NBT::TagCompound &section);
private:
//    ChunkBlocks data;
    Block blocks[65536];
    ArrayAccessor<Block, 16, 16, 256> blocksAccess;
};

}

#endif
