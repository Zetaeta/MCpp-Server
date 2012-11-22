
#include <map>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <math.h>

#include <nbt/NBT.hpp>
#include <nbt/TagCompound.hpp>
#include <nbt/TagNotFound.hpp>
#include <nbt/TagList.hpp>
#include <nbt/TagDouble.hpp>

#include <IOStream/FileInputStream.hpp>
#include <IOStream/GZipInputStream.hpp>
#include <IOStream/DeflateInputStream.hpp>
#include <IOStream/InputStream.hpp>
#include <IOStream/ArrayInputStream.hpp>

#include <Util/StringUtils.hpp>
#include <Util/Rounding.hpp>

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
#include "entity/PlayerData.hpp"
#include "ReentrantLock.hpp"
#include "AutoLock.hpp"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::ostringstream;
using std::shared_ptr;
using std::weak_ptr;
using std::find;
using std::make_shared;
using std::pair;

using NBT::Tag;
using NBT::TagCompound;
using NBT::TagList;
using NBT::TagDouble;

using IOStream::GZipInputStream;
using IOStream::DeflateInputStream;
using IOStream::BIG;
using IOStream::InputStream;
using IOStream::FileInputStream;
using IOStream::ArrayInputStream;

using Util::demangle;
using Util::roundDownToNeg;

using MCServer::Entities::PlayerData;

USING_LOGGING_LEVEL

namespace MCServer {

using Entities::Entity;

struct WorldData {
    string name;
    map<ChunkCoordinates, shared_ptr<Chunk>> chunks;
    ReentrantLock chunksLock;
    vector<shared_ptr<Entity>> entities;
    map<ChunkCoordinates, weak_ptr<Chunk>> unloadingChunks;
    ReentrantLock unloadingChunksLock;

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
    string directory;
};

World::World()
:m(new WorldData) {
}

World::World(const string &name)
:m(new WorldData) {
    m->name = name;
}

World::World(const World &other)
:m(new WorldData(*other.m)) {
}

World::World(World &&other)
:m(other.m) {
    other.m = NULL;
}

World & World::operator=(World &&other){
    m = other.m;
    other.m = NULL;
    return *this;
}

shared_ptr<Chunk> World::chunkAt(int x, int y) const {
    return chunkAt(Point2D(x, y));
}

shared_ptr<Chunk> World::chunkAt(const Point2D &pt) const {
    AUTOLOCK(m->chunksLock);
    auto it = m->chunks.find(pt);
    if (it != m->chunks.end()) {
        return it->second;
    }
    return nullptr;
}

void World::addEntity(const shared_ptr<Entity> &entity) {
    m->entities.push_back(entity);
}

void World::removeEntity(const shared_ptr<Entity> &entity) {
    auto it = find(m->entities.begin(), m->entities.end(), entity);
    if (it == m->entities.end()) {
        throw std::runtime_error("Entity is not there to remove!");
    }
    m->entities.erase(it);
}

vector<std::shared_ptr<Entity>> & World::getEntities() {
    return m->entities;
}

const vector<std::shared_ptr<Entity>> & World::getEntities() const {
    return m->entities;
}

void World::loadFrom(const std::string &directory) {
    AUTOLOCK(m->chunksLock);
    m->directory = directory;
//    cout << "World::loadFrom()\n";
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
//    readRegionFile("world/region/r.0.0.mca");
//    loadChunk({0,0});
}

std::string World::getName() const {
    return m->name;
}

int World::getDimension() const {
    return 0;
}

vector<string> printTag(Tag *tag) {
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
    FileInputStream fin(fileName);
    InputStream in(fin, BIG);
    uint32_t i = in.readInt();
    uint8_t usedSectors = i & 0xFF;
    uint32_t chunkOffset = i >> 8;
    in.seek(chunkOffset * 4096, SEEK_SET);
    int length = in.readInt();
    uint8_t compression = in.readUByte();
    InputStream chunk(new DeflateInputStream(fin), BIG);
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
//    for (size_t i=0; i < tree.size(); ++i) {
//        cout << tree[i];
//    }


    TagCompound &level = compound->getCompound("Level");
    TagList &sections = level.getList("Sections");
    vector<Tag *> &sectionsV = sections.getData();
    for (Tag *section : sectionsV) {
        TagCompound *sectionC = dynamic_cast<TagCompound *>(section);
        uint8_t y = sectionC->getUByte("Y");
        cout << "Y: " << uint16_t(y) << '\n';
    }
//    m->chunks[{0, 0}]->loadFrom(*compound);
    
}

shared_ptr<Chunk> World::loadChunk(const ChunkCoordinates &pos) {
    AUTOLOCK(m->chunksLock);
    auto mbFound = m->chunks.find(pos);
    if (mbFound != m->chunks.end()) {
        cout << "{" << pos.x << ", " << pos.z << "} already loaded!\n";
        return mbFound->second;
    }
    auto ulFound = m->unloadingChunks.find(pos);
    if (ulFound != m->unloadingChunks.end() && !ulFound->second.expired()) {
        auto locked = ulFound->second.lock();
        m->unloadingChunksLock.lock();
        m->unloadingChunks.erase(ulFound);
        m->unloadingChunksLock.unLock();
        m->chunksLock.lock();
        m->chunks[pos] = locked;
        m->chunksLock.unLock();
        return locked;
    }
    ostringstream filenameSs;
    filenameSs << m->directory << "/region/" << "r." << (roundDownToNeg<int, 32>(pos.x) / 32) << '.' << (roundDownToNeg<int, 32>(pos.z) / 32) << ".mca";
    string filename = filenameSs.str();
//    cout << filename << '\n';
    if (!exists(filename)) {
        cout << filename << " does not exist. Creating new chunk at {" << pos.x << ", " << pos.z << "}\n";
        return createChunk(pos);
    }
    FileInputStream fin(filenameSs.str());
    InputStream in(fin, BIG);
    size_t xOffset = pos.x & 31;
    size_t zOffset = pos.z & 31;
    size_t headerOffset = 4 * (xOffset + zOffset * 32); // Offset of chunk information from start of file.
    in.seek(headerOffset, SEEK_SET);
    uint32_t chunkInfo = in.readInt();
    if (chunkInfo == 0) {
        cout << "Creating new chunk at {" << pos.x << ", " << pos.z << "}\n";
        return createChunk(pos);
    }

    uint8_t usedSectors = chunkInfo & 0xFF;
    uint32_t chunkOffset = chunkInfo >> 8;
    in.seek(chunkOffset * 4096, SEEK_SET);
    int length = in.readInt();
    off_t startPos = fin.seek(0, SEEK_CUR);
    uint8_t version = in.readUByte();

    uint8_t *compressedData = new uint8_t[length - 1];
/*    cout << "in.read(): " << */ in.read(compressedData, length - 1);// << '\n';
    DeflateInputStream din(new ArrayInputStream(compressedData, length - 1));
    InputStream compIn(din, BIG);
    TagCompound *chunkRoot = dynamic_cast<TagCompound *>(NBT::readTag(compIn));
    din.close();
    fin.close();
    off_t endPos = fin.seek(0, SEEK_CUR);
    auto chunk = make_shared<Chunk>(pos);
    m->chunks[pos] = chunk;
    chunk->loadFrom(*chunkRoot);
    return chunk;
}

shared_ptr<Chunk> World::createChunk(const ChunkCoordinates &pos) {
    AUTOLOCK(m->chunksLock);
    auto chunk = make_shared<Chunk>(pos);
    m->chunks[pos] = chunk;
    return chunk;
}

void World::unloadChunk(const ChunkCoordinates &pos) {
    auto it = m->chunks.find(pos);
    if (it == m->chunks.end()) {
        throw std::runtime_error("That chunk is not loaded"); 
    }
    unloadChunk(it->second);
}

void World::unloadChunk(const shared_ptr<Chunk> &chunk) {
    AUTOLOCK(m->chunksLock);
    weak_ptr<Chunk> weak(chunk);
    auto coords = chunk->getCoordinates();
    auto it = m->chunks.find(coords);
    if (it != m->chunks.end()) {
        m->chunks.erase(it);
    }
    if (!weak.expired()) {
        m->unloadingChunksLock.lock();
        m->unloadingChunks[coords] = weak;
        m->unloadingChunksLock.unLock();
    }
}

void World::saveChunk(const ChunkCoordinates &coords) {
    auto it = m->chunks.find(coords);
    if (it == m->chunks.end()) {
        throw std::runtime_error("That chunk is not loaded"); 
    }
    saveChunk(it->second);
}

void World::saveChunk(const shared_ptr<Chunk> &chunk) {
    AUTOLOCK(m->chunksLock);
    ChunkCoordinates coords = chunk->getCoordinates();
    ostringstream filenameSs;
    filenameSs << m->directory << "/region/r." << (roundDownToNeg<int, 32>(coords.x) / 32)
        << '.' << (roundDownToNeg<int, 32>(coords.z) / 32) << ".mca";
    string filename = filenameSs.str();
    if (!exists(filename)) {
        
    }
}

void World::loadPlayer(PlayerData *data) {
    cout << "loadPlayer(): " << data->name << '\n';
    cout << "World's directory: " << m->directory << '\n';
    string filename = m->directory + "/players/" + data->name + ".dat";
    if (!exists(filename)) {
        return;
    }
    InputStream in(new GZipInputStream(filename));
    Tag *tag = NBT::readTag(in);

    cout << "Player data: \n";
    vector<string> stuff = printTag(tag);

    TagCompound *root = dynamic_cast<TagCompound *>(tag);
    data->onGround = root->getByte("onGround");
    data->sleeping = root->getByte("Sleeping");
    data->air = root->getShort("Air");
    data->attackTime = root->getShort("AttackTime");
    data->deathTime = root->getShort("DeathTime");
    data->fire = root->getShort("Fire");
    data->health = root->getShort("Health");
    data->hurtTime = root->getShort("HurtTime");
    data->sleepTimer = root->getShort("SleepTimer");
    data->dimension = root->getInt("Dimension");
    data->foodLevel = root->getInt("foodLevel");
    data->foodTickTimer = root->getInt("foodTickTimer");
    data->playerGameType = GameMode(root->getInt("playerGameType"));
    data->xpLevel = root->getInt("XpLevel");
    data->xpTotal = root->getInt("XpTotal");
    data->fallDistance = root->getFloat("FallDistance");
    data->foodExhaustionLevel = root->getFloat("foodExhaustionLevel");
    data->foodSaturationLevel = root->getFloat("foodSaturationLevel");
    data->xpP = root->getFloat("XpP");
    TagList position = root->getList("Pos");
    data->position = Point3D{ dynamic_cast<TagDouble *>(position[0])->getData(),
                      dynamic_cast<TagDouble *>(position[1])->getData(),
                      dynamic_cast<TagDouble *>(position[2])->getData()};
}


template <typename T, typename Stream>
Stream & operator<<(Stream &strm, const vector<T> &vec) {
    strm << '{';
    for_each(vec.begin(), vec.end(), [&] (const T &t) {
        strm << t << ", ";
    });
    strm << '}';
    return strm;
}


vector<shared_ptr<Chunk>> World::loadAll(const vector<ChunkCoordinates> &coords) {
    cout << "World::loadAll():\n" << coords << '\n';
    vector<shared_ptr<Chunk>> returned(coords.size());
    for (size_t i=0; i<coords.size(); ++i) {
        returned[i] = loadChunk(coords[i]);
    }
    return returned;
}

vector<shared_ptr<Chunk>> World::getLoadedChunks() const {
    vector<shared_ptr<Chunk>> returned;
    for (const pair<ChunkCoordinates, shared_ptr<Chunk>> pair : m->chunks) {
        returned.push_back(pair.second);
    }
    return returned;
}

map<ChunkCoordinates, weak_ptr<Chunk>> & World::getUnloadingChunks() {
    return m->unloadingChunks;
}

const map<ChunkCoordinates, weak_ptr<Chunk>> & World::getUnloadingChunks() const {
    return m->unloadingChunks;
}

ReentrantLock & World::getChunksLock() {
    return m->chunksLock;
}

ReentrantLock & World::getUnloadingChunksLock() {
    return m->unloadingChunksLock;
}

}

