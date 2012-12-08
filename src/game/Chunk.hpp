
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block/Block.hpp"
#include "util/ArrayAccessor.hpp"
#include "ChunkCoordinates.hpp"

namespace NBT {
class TagCompound;
}

namespace MCServer {

namespace Network {
class ClientConnection;
}

typedef ArrayAccessor<Block, 16, 16, 256> ChunkData, BlocksXZY, Blocks3D;
typedef ArrayAccessor<Block, 16, 256> BlocksZY, Blocks2D;
typedef ArrayAccessor<Block, 256> BlocksY, Blocks1D;

class Chunk {
public:
    Chunk(const ChunkCoordinates &);
    ArrayAccessor<Block, 16, 256> operator[](size_t index) {
//        return blocksAccess[index];
        
    }

    ArrayAccessor<const Block, 16, 256> operator[](size_t index) const {
        return blocksAccess[index];
    }

    ChunkCoordinates getCoordinates() const {
        return coords;
    }

    void loadFrom(const NBT::TagCompound &compound);
    void loadSection(const NBT::TagCompound &section);

    friend class Network::ClientConnection;
private:
    Chunk(const Chunk &);
//    ChunkBlocks data;
    uint8_t blockIds[65536];
    uint8_t blockMetaData[65536 / 2];
    ArrayAccessor<uint8_t, 16, 16, 256> blockIdAccess;
    ArrayAccessor<uint8_t, 16, 16, 256> blockMetaDataAccess;
    ChunkCoordinates coords;
};

}

#endif
