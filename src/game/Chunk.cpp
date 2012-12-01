
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>

#include <nbt/TagCompound.hpp>
#include <nbt/TagList.hpp>
#include <nbt/TagNotFound.hpp>

#include "Chunk.hpp"
#include "util/Utils.hpp"

using std::cout;
using std::vector;

using NBT::TagCompound;
using NBT::TagList;
using NBT::Tag;

namespace MCServer {

Chunk::Chunk(const ChunkCoordinates &coords)
:blocksAccess(blocks), coords(coords) {
}

Chunk::Chunk(const Chunk &other)
#ifdef __GNUG___
:blocks(other.blocks), blocksAccess(blocks) {
#else
:blocksAccess(blocks) {
    memcpy(blocks, other.blocks, 65536);
#endif
}

void Chunk::loadFrom(const TagCompound &compound) {
    const TagCompound &level = compound.getCompound("Level");
    const TagList &sections = level.getList("Sections");
    const vector<Tag *> sectionsV = sections.getData();
//    cout << "Chunk::loadFrom: sectionsV.size() = " << sectionsV.size() << '\n';
    for (Tag *section : sectionsV) {
        const TagCompound *sectionC = dynamic_cast<const TagCompound *>(section);
        loadSection(*sectionC);
    }
}

void Chunk::loadSection(const TagCompound &section) {
    uint8_t height = section.getByte("Y");
//    if (height != 3 || height != 2) {
//        return;
//    }
    vector<uint8_t> blocks;
    try {
        blocks = section.getByteArray("Blocks");
    }
    catch (const NBT::TagNotFound &) {
        return;
    }
//    cout << "Chunk::loadSection: Y = " << uint16_t(height) << '\n';
//    cout << "blocks.size(): " << blocks.size() << '\n';


//#define LOOP_COPY
#ifdef LOOP_COPY
    ArrayAccessor<uint8_t, 16, 16, 16> blocksIn = blocks.data();
    ArrayAccessor<Block, 16, 16, 16> blocksOut = &this->blocks[height * 256];

    for (int x=0; x<16; ++x) {
        for (int z=0; z<16; ++z) {
            for (int y=0; y<16; ++y) {
                blocksOut[x][y][z] = blocksIn[x][y][z];
            }
        }
    }
#else
    Block *bOut = &this->blocks[height * 4096];
    uint8_t *bIn = blocks.data();
    for (int i=0; i<4096; ++i) {
        bOut[i].id = bIn[i];
    }
//    cout << "bOut - blocks = " << (bOut - this->blocks) << '\n';
#endif

    const vector<uint8_t> data = section.getByteArray("Data");

    cout << "data.size() = " << data.size() << '\n';
    for (int i=0; i<2048; ++i) {
        bOut[i * 2].metadata = data[i] >> 8;
        bOut[i * 2 + 1].metadata = data[i] & 0xFF;
    }
}

}
