
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
:blockIds(other.blockIds), blockMetaData(other.blockMetaData), blocksAccess(blocks) {
#else
:blocksAccess(blocks) {
    memcpy(blockIds, other.blockIds, 65536);
    memcpy(blockMetaData, other.blockMetaData, 65536);
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
    vector<uint8_t> blocks;
    try {
        blocks = section.getByteArray("Blocks");
    }
    catch (const NBT::TagNotFound &) {
        return;
    }


    memcpy(&blockIds[height * 4096], blocks.data(), 4096)

    const vector<uint8_t> data = section.getByteArray("Data");

    cout << "data.size() = " << data.size() << '\n';
    for (int i=0; i<2048; ++i) {
        blockMetaData[i * 2] = data[i] >> 8;
        blockMetaData[i * 2 + 1] = data[i] & 0xFF;
    }
}

}
