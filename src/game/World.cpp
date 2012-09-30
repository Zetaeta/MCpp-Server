
#include <map>
#include <typeinfo>
#include <iostream>
#include <sstream>

#include <nbt/NBT.hpp>
#include <nbt/TagCompound.hpp>
#include <nbt/TagNotFound.hpp>
#include <nbt/TagList.hpp>

#include <IOStream/PlainInputStream.hpp>
#include <IOStream/GZipInputStream.hpp>
#include <IOStream/DeflateInputStream.hpp>
#include <IOStream/InputStream.hpp>

#include "World.hpp"
#include "Point3D.hpp"
#include "ChunkCoordinates.hpp"
#include "Chunk.hpp"
#include "util/FSUtils.hpp"
#include "GameMode.hpp"
#include "MinecraftServer.hpp"
#include "logging/Logger.hpp"
#include "WorldLoadingFailure.hpp"
#include "util/Utils.hpp"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::ostringstream;

using NBT::Tag;
using NBT::TagCompound;
using NBT::TagList;

using IOStream::PlainInputStream;
using IOStream::GZipInputStream;
using IOStream::DeflateInputStream;
using IOStream::BIG;
using IOStream::InputStream;

USING_LOGGING_LEVEL

namespace MCServer {

struct WorldData {
    string name;
    map<ChunkCoordinates, Chunk> chunks;

    bool hardcore;
    bool structures;
    bool raining;
    int rainTime;
    bool thundering;
    int thunderTime;
    GameMode gameMode;
    int generatorVersion;
    Point3D spawnPoint;
    int version;
    long lastPlayed;
    long randomSeed;
    long sizeOnDisk;
    long time;

    string generatorName;
};

World::World()
:m(new WorldData) {
}

World::World(const string &name)
:m(new WorldData) {
    m->name = name;
}

Chunk & World::chunkAt(int x, int y) {
    return chunkAt(Point2D(x, y));
}

Chunk & World::chunkAt(const Point2D &pt) {
    return m->chunks[pt];
}

void World::loadFrom(const std::string &directory) {
    cout << "World::loadFrom()\n";
    string levelDatFile = directory + "/level.dat";
    if (!exists(levelDatFile)) {
        MinecraftServer::getServer().getLogger() << "Error: missing level.dat file in " << directory << '\n';
        return;
    }
    cout << "Loading level.dat...\n";
    Tag *tag = NBT::readFromFile(levelDatFile);
    TagCompound *root = dynamic_cast<TagCompound *>(tag);
    if (!root) {
        MinecraftServer::getServer().getLogger() << SEVERE << "Failed loading world in " << directory << ": Root of level.dat is not a TagCompound\n";
        return;
    }
    cout << "Loaded root! Loading contents...\n";

    try {
        TagCompound &data = root->getCompound("Data");
        m->hardcore           = data.getByte("hardcore");
        m->structures        = data.getByte("MapFeatures");
        m->raining            = data.getByte("raining");
        m->thundering         = data.getByte("thundering");
        m->gameMode            = GameMode(data.getInt("GameType"));
        m->generatorVersion    = data.getInt("generatorVersion");
        m->rainTime            = data.getInt("rainTime");
        int spawnX              = data.getInt("SpawnX");
        int spawnY              = data.getInt("SpawnY");
        int spawnZ              = data.getInt("SpawnZ");
        m->spawnPoint = Point3D(spawnX, spawnY, spawnZ);
        m->thunderTime         = data.getInt("thunderTime");
        m->version             = data.getInt("version");
        m->lastPlayed         = data.getLong("LastPlayed");
        m->randomSeed         = data.getLong("RandomSeed");
        m->sizeOnDisk         = data.getLong("SizeOnDisk");
        m->time               = data.getLong("Time");
        m->generatorName    = data.getString("generatorName");
        m->name        = data.getString("LevelName");
        cout << "Loaded contents!\n";
    } catch (const NBT::TagNotFound &ex) {
        cout << "Failed loading!\n";
        throw WorldLoadingFailure(string("Missing ") + ex.what() + " from level.dat!");
    }
    cout << "Loaded level.dat! Loading region files...\n";
    vector<string> regionFiles = getEntries(directory + "/region");
    for (auto it = regionFiles.begin(); it != regionFiles.end(); ++it) {
        cout << "Found region file " << *it << '\n';
//        readRegionFile(*it);
    }
    readRegionFile("world/region/r.0.0.mca");
}

const std::string & World::getName() {
    return m->name;
}

vector<string> printTag(Tag *tag) {
    cout << "tag = " << tag << '\n';
    vector<string> strings;
    std::ostringstream ss;
    try {
        ss << demangle(typeid(*tag).name());
    } catch (const std::exception &e) {
        cout << e.what() << "tag = " << tag << '\n';
    }
    if (tag->getName().size() > 0) {
        ss << ": " << tag->getName();
    }
    TagCompound *compound = 0;
    TagList *list = 0;
    if ((compound = dynamic_cast<TagCompound *>(tag)) != 0) {
        ss << " {\n";
        strings.push_back(ss.str());
        map<string, Tag *> &m = compound->getData();
        for (auto it = m.begin(); it != m.end(); ++it) {
            vector<string> child = printTag(it->second);
            for (auto it2 = child.begin(); it2 != child.end(); ++it2) {
                strings.push_back("    " + *it2);
            }
        }
        strings.push_back("}\n");
    }
    else if ((list = dynamic_cast<TagList *>(tag)) != 0) {
        ss << "{\n";
        strings.push_back(ss.str());
        vector<Tag *> &m = list->getData();
        for (auto it = m.begin(); it != m.end(); ++it) {
            cout << "it: " << *it << '\n';
            if (!*it) {
                strings.push_back("NULL");
                continue;
            }
            vector<string> child = printTag(*it);
            for (auto it2 = child.begin(); it2 != child.end(); ++it2) {
                strings.push_back("    " + *it2);
            }
        }
        strings.push_back("}\n");
    }
    else {
        ss << '\n';
        strings.push_back(ss.str());
    }
    return strings;
}

void World::readRegionFile(const std::string &fileName) {
    PlainInputStream _in(fileName);
    InputStream in(_in, BIG);
    uint32_t i = in.readInt();
    uint8_t usedSectors = i & 0xFF;
    uint32_t chunkOffset = i >> 8;
    in.seek(chunkOffset * 4096, SEEK_SET);
    int length = in.readInt();
    uint8_t compression = in.readUByte();
    DeflateInputStream _chunk(_in.fd());
    InputStream chunk(_chunk, BIG);
    Tag *chunkRoot = NBT::readTag(chunk);
    cout << "Loaded chunkRoot: " << chunkRoot << "\n";
    TagCompound *compound = dynamic_cast<TagCompound *>(chunkRoot);
    cout << "Compound's type: \n";
    cout << "    " << demangle(typeid(*compound).name()) << '\n';
    cout << "Compound's name: \n";
    cout << "    " << compound->getName() << '\n';

    cout << "compound's members: \n";
    cout << "    compound: " << compound << '\n';
    vector<string> tree = printTag(compound);
    for (size_t i=0; i < tree.size(); ++i) {
        cout << tree[i];
    }


    TagCompound &level = compound->getCompound("Level");
    TagList &sections = level.getList("Sections");
    vector<Tag *> &sectionsV = sections.getData();
    for (Tag *section : sectionsV) {
        TagCompound *sectionC = dynamic_cast<TagCompound *>(section);
        uint8_t y = sectionC->getUByte("Y");
        cout << "Y: " << uint16_t(y) << '\n';
    }
    m->chunks[{0, 0}].loadFrom(*compound);
    
}

Chunk & World::loadChunk(const ChunkCoordinates &pos) {
    ostringstream filenameSs;
    filenameSs << "r." << (pos.x / 32) << '.' << (pos.z / 32) << ".mca";
    string filename = filenameSs.str();
    InputStream plain(new PlainInputStream(filenameSs.str()), BIG);
    size_t headerOffset = 4 * (pos.x + pos.z * 32); // Offset of chunk information from start of file.
    plain.seek(headerOffset, SEEK_SET);
}

}

