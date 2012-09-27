
#include <map>
#include <typeinfo>
#include <iostream>

#include <nbt/NBT.hpp>
#include <nbt/TagCompound.hpp>
#include <nbt/TagNotFound.hpp>

#include <IOStream/PlainInputStream.hpp>
#include <IOStream/GZipInputStream.hpp>
#include <IOStream/DeflateInputStream.hpp>

#include "World.hpp"
#include "Point3D.hpp"
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

using NBT::Tag;
using NBT::TagCompound;

using IOStream::PlainInputStream;
using IOStream::GZipInputStream;
using IOStream::DeflateInputStream;
using IOStream::BIG;

USING_LOGGING_LEVEL

namespace MCServer {

struct WorldData {
    string name;
    map<Point2D, Chunk> chunks;

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

World::World(const string &name, uint128_t uid)
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

void World::readRegionFile(const std::string &fileName) {
    PlainInputStream in(fileName, BIG);
    uint32_t i = in.readInt();
    uint8_t usedSectors = i & 0xFF;
    uint32_t chunkOffset = i >> 8;
    cout << "Loading " << fileName << '\n';
    cout << "Sectors used by chunk: " << uint16_t(usedSectors) << ", chunk offset: " << chunkOffset << '\n';
    in.seek(chunkOffset * 4096, SEEK_SET);
    cout << "File size: " << fileSize(fileName) << '\n';
    int length = in.readInt();
    uint8_t compression = in.readUByte();
    cout << "Length: " << length << ", compression = " << uint16_t(compression) << '\n';
    DeflateInputStream chunk(in.fd(), BIG);
    Tag *chunkRoot = NBT::readTag(chunk);
    cout << "Loaded chunkRoot: " << chunkRoot << "\n";
    TagCompound *compound = dynamic_cast<TagCompound *>(chunkRoot);
    cout << "Compound's type: \n";
    cout << "    " << demangle(typeid(*compound).name()) << '\n';
    cout << "Compound's name: \n";
    cout << "    " << compound->getName() << '\n';
    map<string, NBT::Tag *> &chunkData = compound->getData();
    cout << "compound's members: \n";
    for (auto it = chunkData.begin(); it != chunkData.end(); ++it) {
        cout << "    " << it->first << ": " << demangle(typeid(*it->second).name()) << '\n';
    }

}

}

