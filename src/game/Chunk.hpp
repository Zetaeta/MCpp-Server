
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block/Block.hpp"
#include "util/ArrayAccessor.hpp"
#include "ChunkCoordinates.hpp"
#include "BlockAccess.hpp"

namespace NBT {
class TagCompound;
}

namespace MCServer {

namespace Network {
class ClientConnection;
}


class Chunk {
public:
    Chunk(const ChunkCoordinates &);

    /**
     * Returns a BlockAccess that can be further indexed to to get a particular block.
     * Usage:
     *     To get a block at {x,y,z}:
     *     Block b = chunk[x][z][y];
     */
    BlockAccess<2> operator[](size_t x) {
        return BlockAccess<2>(*this, x);
    }

    ConstBlockAccess<2> operator[](size_t x) const {
        return ConstBlockAccess<2>(*this, x);
    }

    ChunkCoordinates getCoordinates() const {
        return coords;
    }

    void loadFrom(const NBT::TagCompound &compound);
    void loadSection(const NBT::TagCompound &section);

    friend class Network::ClientConnection;
    friend class Block;
private:
    Chunk(const Chunk &);
//    ChunkBlocks data;
    /**
     * The index of a block at x,y,z is x + z * 16 + y * 256
     */
    uint8_t blockIds[65536];
    uint8_t blockMetadata[65536 / 2];
    ChunkCoordinates coords;
};

inline constexpr int index(uint8_t x, uint8_t y, uint8_t z) {
    return x + (z << 4) + (y << 8);
}

}

#endif
