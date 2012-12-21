
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <memory>

#include <nbt/TagCompound.hpp>
#include <nbt/TagList.hpp>
#include <nbt/TagNotFound.hpp>

#include "Chunk.hpp"
#include "util/Utils.hpp"

using std::cout;
using std::vector;
using std::shared_ptr;
using std::dynamic_pointer_cast;

using NBT::TagCompound;
using NBT::TagList;
using NBT::Tag;

namespace MCServer {

Chunk::Chunk(const ChunkCoordinates &coords)
:coords(coords) {
}

Chunk::Chunk(const Chunk &other)
#ifdef __GNUG___
:blockIds(other.blockIds), blockMetadata(other.blockMetadata) {
#else
{
    memcpy(blockIds, other.blockIds, 65536);
    memcpy(blockMetadata, other.blockMetadata, 65536);
#endif
}

void Chunk::loadFrom(const TagCompound &compound) {
    shared_ptr<TagCompound> level = compound.getCompound("Level");
    shared_ptr<TagList> sections = level->getList("Sections");
    const vector<shared_ptr<Tag>> sectionsV = sections->getData();
//    cout << "Chunk::loadFrom: sectionsV.size() = " << sectionsV.size() << '\n';
    for (const shared_ptr<Tag> section : sectionsV) {
        shared_ptr<TagCompound> sectionC = dynamic_pointer_cast<TagCompound>(section);
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


    memcpy(&blockIds[height * 4096], blocks.data(), 4096);

    const vector<uint8_t> data = section.getByteArray("Data");

    cout << "data.size() = " << data.size() << '\n';
    for (int i=0; i<2048; ++i) {
        blockMetadata[i * 2] = data[i] >> 8;
        blockMetadata[i * 2 + 1] = data[i] & 0xFF;
    }
}

}
