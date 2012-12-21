
#include <map>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <math.h>
#include <sys/stat.h>

#include <nbt/NBT.hpp>
#include <nbt/TagCompound.hpp>
#include <nbt/TagNotFound.hpp>
#include <nbt/TagList.hpp>
#include <nbt/TagDouble.hpp>
#include <nbt/TagByteArray.hpp>

#include <IOStream/FileInputStream.hpp>
#include <IOStream/FileOutputStream.hpp>
#include <IOStream/GZipInputStream.hpp>
#include <IOStream/DeflateInputStream.hpp>
#include <IOStream/InputStream.hpp>
#include <IOStream/OutputStream.hpp>
#include <IOStream/InputOutputStream.hpp>
#include <IOStream/ArrayInputStream.hpp>
#include <IOStream/ArrayOutputStream.hpp>
#include <IOStream/DeflateOutputStream.hpp>
#include <IOStream/FileIOStream.hpp>

#include <Util/StringUtils.hpp>
#include <Util/Rounding.hpp>
#include <Util/Endian.h>
#include <Util/ErrorHandler.hpp>

#include "World.hpp"
#include "ReentrantLock.hpp"
#include "AutoLock.hpp"
#include "MinecraftServer.hpp"
#include "Point3D.hpp"
#include "ChunkCoordinates.hpp"
#include "Chunk.hpp"
#include "GameMode.hpp"
#include "WorldLoadingFailure.hpp"
#include "util/FSUtils.hpp"
#include "logging/Logger.hpp"
#include "util/Utils.hpp"
#include "entity/PlayerData.hpp"
#include "entity/EntityItem.hpp"
#include "entity/Player.hpp"
#include "network/ClientConnection.hpp"

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
using std::dynamic_pointer_cast;

using NBT::Tag;
using NBT::TagCompound;
using NBT::TagList;
using NBT::TagDouble;
using NBT::TagByteArray;

using IOStream::GZipInputStream;
using IOStream::DeflateInputStream;
using IOStream::DeflateOutputStream;
using IOStream::BIG;
using IOStream::InputStream;
using IOStream::OutputStream;
using IOStream::InputOutputStream;
using IOStream::FileInputStream;
using IOStream::FileOutputStream;
using IOStream::FileIOStream;
using IOStream::ArrayInputStream;
using IOStream::ArrayOutputStream;

using Util::demangle;
using Util::roundDownToNeg;
using Util::roundUp;

using MCServer::Entities::PlayerData;

USING_LOGGING_LEVEL

namespace MCServer {

using Entities::Entity;
using Entities::EntityItem;
using Entities::Player;

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


inline int World::chunkToRegion(int chunkCoord) {
//    return roundDownToNeg<int, 32>(chunkCoord) / 32;
    return floor(chunkCoord / 32);
}

inline size_t World::headerOffset(int x, int z) {
    return static_cast<size_t>(x & 31) + static_cast<size_t>(z & 31) * 32;
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
    string levelDatFile = directory + "/level.dat";
    if (!exists(levelDatFile)) {
        MinecraftServer::getServer().getLogger() << "Error: missing level.dat file in " << directory << '\n';
        return;
    }
    cout << "Loading level.dat...\n";
    shared_ptr<Tag> tag = NBT::readFromFile(levelDatFile);
    shared_ptr<TagCompound> root = dynamic_pointer_cast<TagCompound>(tag);
    if (!root) {
        MinecraftServer::getServer().getLogger() << SEVERE << "Failed loading world in " << directory << ": Root of level.dat is not a TagCompound\n";
        return;
    }
    cout << "Loaded root! Loading contents...\n";

    try {
        shared_ptr<TagCompound> data = root->getCompound("Data");
        m->hardcore           = data->getByte("hardcore");
        m->structures        = data->getByte("MapFeatures");
        m->raining            = data->getByte("raining");
        m->thundering         = data->getByte("thundering");
        m->gameMode            = GameMode(data->getInt("GameType"));
        m->generatorVersion    = data->getInt("generatorVersion");
        m->rainTime            = data->getInt("rainTime");
        int spawnX              = data->getInt("SpawnX");
        int spawnY              = data->getInt("SpawnY");
        int spawnZ              = data->getInt("SpawnZ");
        m->spawnPoint = Point3D(spawnX, spawnY, spawnZ);
        m->thunderTime         = data->getInt("thunderTime");
        m->version             = data->getInt("version");
        m->lastPlayed         = data->getLong("LastPlayed");
        m->randomSeed         = data->getLong("RandomSeed");
        m->sizeOnDisk         = data->getLong("SizeOnDisk");
        m->time               = data->getLong("Time");
        m->generatorName    = data->getString("generatorName");
        m->name        = data->getString("LevelName");
        cout << "Loaded contents!\n";
    } catch (const NBT::TagNotFound &ex) {
        cout << "Failed loading!\n";
        throw WorldLoadingFailure(string("Missing ") + ex.what() + " from level.dat!");
    }
    cout << "Loaded level.dat! Loading region files...\n";
    vector<string> regionFiles = getEntries(directory + "/region");
    for (auto it = regionFiles.begin(); it != regionFiles.end(); ++it) {
        cout << "Found region file " << *it << '\n';
    }
}

std::string World::getName() const {
    return m->name;
}

int World::getDimension() const {
    return 0;
}

/*
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
        map<string, shared_ptr<Tag>> &m = compound->getData();
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
*/

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
    shared_ptr<Tag> chunkRoot = NBT::readTag(chunk);
    cout << "Loaded chunkRoot: " << chunkRoot << "\n";
    shared_ptr<TagCompound> compound = dynamic_pointer_cast<TagCompound>(chunkRoot);
    cout << "Compound's type: \n";
    cout << "    " << demangle(typeid(*compound).name()) << '\n';
    cout << "Compound's name: \n";
    cout << "    " << compound->getName() << '\n';

    cout << "compound's members: \n";
    cout << "    compound: " << compound << '\n';

    shared_ptr<TagCompound> level = compound->getCompound("Level");
    shared_ptr<TagList> sections = level->getList("Sections");
    vector<shared_ptr<Tag>> &sectionsV = sections->getData();
    for (const auto &section : sectionsV) {
        shared_ptr<TagCompound> sectionC = dynamic_pointer_cast<TagCompound>(section);
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
    filenameSs << m->directory << "/region/" << "r." << chunkToRegion(pos.x) << '.' << chunkToRegion(pos.z) << ".mca";
    string filename = filenameSs.str();
//    cout << filename << '\n';
    if (!exists(filename)) {
        cout << filename << " does not exist. Creating new chunk at {" << pos.x << ", " << pos.z << "}\n";
        return createChunk(pos);
    }
    FileInputStream fin(filenameSs.str());
    InputStream in(fin, BIG);
    in.seek(headerOffset(pos.x, pos.z) * 4, SEEK_SET);
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
    shared_ptr<TagCompound> chunkRoot = dynamic_pointer_cast<TagCompound>(NBT::readTag(compIn));
//    cout << "chunkRoot.getName() = " << chunkRoot->getName() << '\n';
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
    saveChunk(chunk);
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
    cout << "saving " << coords << '\n';

    TagCompound root;
    TagCompound level("Level");
    level.set("xPos", (int) coords.x);
    level.set("zPos", (int) coords.z);
    level.set("TerrainPopulated", (uint8_t) 1);
    auto sections = make_shared<TagList>("Sections");
    for (int y=0; y<16; ++y) {
        auto section = make_shared<TagCompound>();
        section->set("Y", uint8_t(y));
        auto blocks = make_shared<TagByteArray>(&chunk->blockIds[4096 * y], 4096, "Blocks");
        section->set(blocks);
        auto data = make_shared<TagByteArray>(&chunk->blockMetadata[2048 * y], 2048, "Data");
        section->set(data);
        sections->add(section);
    }
    level.set(sections);
    root.set(level);

    ArrayOutputStream aout(4096);
    DeflateOutputStream dout(aout); 
    OutputStream out(dout, BIG);
    NBT::writeTag(root, out);
    dout.close();

/*    {
        ArrayOutputStream testAout(8192);
        OutputStream testOut(testAout, BIG);
        root.write(testOut);
        ArrayInputStream testAin(testAout.data(), testAout.size());
        InputStream testIn(testAin, BIG);
        NBT::readTag(testIn);
    } */


    ArrayInputStream ain(aout.data(), aout.size());
    DeflateInputStream din(ain);
    InputStream in(din);
    NBT::readTag(in);
    din.close();
    ain.close();


    ostringstream filenameSs;
    filenameSs << m->directory << "/region/r." << chunkToRegion(coords.x)
        << '.' << chunkToRegion(coords.z) << ".mca";
    string filename = filenameSs.str();
    cout << "saving to " << filename << '\n';
    FileIOStream fs(filename);
    InputOutputStream file(fs);
    struct stat st;
    if (fstat(fs.fd(), &st) != 0) {
        throwException(errno);
    }
    cout << "file size = " << st.st_size << '\n';
    if (st.st_size == 0) {
        ftruncate(fs.fd(), 4096);
    }
    if (st.st_size % 4096) { // File size must be a multiple of 4kB
        ftruncate(fs.fd(), roundUp<off_t, 4096>(st.st_size));
    }
    unsigned int offsetTable[1024]; // Header of chunk offsets
/*
    file.read(&offsetTable, sizeof(offsetTable)); // Read into offset table. Table now contains big endian integers.
    if (!bigEndian) {
        for (int i=0; i<1024; ++i) {
            swapEndian(offsetTable[i]);
        }
    }
*/
//    cout << "current position in file: " << file.seek(0, SEEK_CUR) << '\n';
    for (int i=0; i<1024; ++i) {
        offsetTable[i] = file.readUInt();
//        std::cout << "offsetTable[" << i << "] = " << offsetTable[i] << '\n';
//        std::cout << "offset = " << (offsetTable[i] >> 8) << '\n';
//        std::cout << "sectors used = " << (offsetTable[i] & 0xFF) << '\n';
    }

    unsigned &offset = offsetTable[headerOffset(coords.x, coords.z)];
    int headerInfo = file.readInt();
    int usedSectors = headerInfo & 0xFF;
    int requiredSectors = aout.size() + 5 / 4096 + 1; // 5 = length of chunk header.
                                                      // division rounds down, so add 1.
    if (requiredSectors <= usedSectors) { // We have the enough sectors already :D
        file.writeInt(aout.size());        
        file.writeByte(2);
        file.write(aout.data(), aout.size());
    }
    else {
        offset = 0; // Set the offset for this chunk to 0, as we no longer need its space.

        assert(fstat(fs.fd(), &st) == 0);
        int sectorCount = st.st_size / 4096 - 1; // Number of sectors in file, minus the header.
        vector<bool> sectors(sectorCount);

        // For all the chunks saved, mark their sectors as used.
        // Henceforth, sectors are indexed from the first sector after the offset table header.
        for (int i=0; i<1024; ++i) {
            unsigned data = offsetTable[i];
            int offset = (data >> 8) - 1; // The offset includes the header, so subtract 1.
            int usedSectors = data & 0xFF;
            if (offset + usedSectors - 1 == sectorCount) {
                cout << "offset + usedSectors - 1 == sectorCount:" << '\n';
                cout << "[" << i << "]: offset = " << offset << ", usedSectors = " << usedSectors << ", sectorCount = " << sectorCount << '\n';
            }
            // This shouldn't happen, but I've seen it in a file that was created by a Notchian server.
            if (offset + usedSectors - 1 > sectorCount) {
                cout << "offset + usedSectors > sectorCount:" << '\n';
                cout << "[" << i << "]: offset = " << offset << ", usedSectors = " << usedSectors << ", sectorCount = " << sectorCount << '\n';
                continue;
            }
            for (int j=offset; j < offset + usedSectors; ++j) {
                sectors[j] = true;
            }
        }

        /* For all the sectors, if there is a row of them free greater than the number required, check them against the current
        lowest. We want the smallest free space that is large enough for our needs, in order to save space. */
        int currentIndex = -1;
        int currentLength = -1;
        for (int i=0; i < sectorCount; ++i) {
            int freeCount = 0; // Current no. of consecutive free sectors.
            int startIndex = i;
            while (i < sectorCount && !sectors[i]) {
                ++freeCount;
                ++i;
            }
            if (freeCount >= requiredSectors) { // If the free area is at least the size we need.
                if (currentLength == -1 || freeCount < currentLength) {
                    currentIndex = startIndex;
                    currentLength = freeCount;
                }
            }
        }
        if (currentIndex >= 0) { // There is enough space in the file.
            file.seek(headerOffset(coords.x, coords.z) * 4, SEEK_SET);
            file.writeInt(((currentIndex + 1) << 8) | (requiredSectors & 0xFF));
            file.seek(currentIndex * 4096 + 4096, SEEK_SET);
            file.writeInt(aout.size());
            file.writeByte(2);
            file.write(aout.data(), aout.size());
        }
        else { // We need to make the file bigger!
            file.seek(headerOffset(coords.x, coords.z) * 4, SEEK_SET);
            file.writeInt((((roundUp<int, 4096>(st.st_size) / 4096) << 8) | (requiredSectors & 0xFF)));
            file.seek(0, SEEK_END);
            file.writeInt(aout.size());
            file.writeByte(2);
            file.write(aout.data(), aout.size());
        }
        // Hopefully-maybe-done?
    }

}

void World::saveAll() {
    for (const auto &pair : m->chunks) {
        saveChunk(pair.second);
    }
}

shared_ptr<Tag> makeTag() {
    return make_shared<TagDouble>();
}

void World::loadPlayer(PlayerData *data) {
    cout << "loadPlayer(): " << data->name << '\n';
    cout << "World's directory: " << m->directory << '\n';
    string filename = m->directory + "/players/" + data->name + ".dat";
    if (!exists(filename)) {
        return;
    }
    InputStream in(new GZipInputStream(filename));
    shared_ptr<Tag> tag = NBT::readTag(in);

    shared_ptr<TagCompound> root = dynamic_pointer_cast<TagCompound>(tag);
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
    shared_ptr<TagList> position = root->getList("Pos");
    auto m1 = (*position)[0];
//    cout << "m1.use_count(): " << m1.use_count() << '\n';
    auto tagDouble = NBT::createTag(6);
//    cout << "tagDouble.use_count(): " << tagDouble.use_count() << '\n';
    auto madeShared = make_shared<TagDouble>();
    cout << "madeShared.use_count(): " << madeShared.use_count() << '\n';
    shared_ptr<Tag> cast = make_shared<TagDouble>();
    cout << "cast.use_count(): " << cast.use_count() << '\n';
    auto returned = makeTag();
    cout << "returned.use_count(): " << returned.use_count() << '\n';
    data->position = Point3D{ dynamic_pointer_cast<TagDouble>((*position)[0])->getData(),
                      dynamic_pointer_cast<TagDouble>((*position)[1])->getData(),
                      dynamic_pointer_cast<TagDouble>((*position)[2])->getData()};
}

void World::entityMoved(Entity &entity, Point3D from, Point3D to) {

}

shared_ptr<EntityItem> World::dropItem(const ItemStack &items, Point3D location) {
    shared_ptr<EntityItem> item = make_shared<EntityItem>(items.getId(), items.size(), location);
    addEntity(item);
    for (const shared_ptr<Entity> &entity : m->entities) {
        shared_ptr<Player> player = dynamic_pointer_cast<Player>(entity);
        if (!player)
            continue;
        cout << "Found player in world!\n";
        cout << player->getPosition() << " - " << location << " = " << player->getPosition().distanceTo3D(location) << '\n';
        if (player->getPosition().distanceTo3D(location) < Player::ITEM_VIEW_DISTANCE) {
            cout << "Found player in world near to item!\n";
            player->getConnection().sendItemSpawned(item);
        }
    }
    return item;
}

template <typename T, typename Stream>
Stream & operator<<(Stream &strm, const vector<T> &vec) {
    strm << '{';
    for (const T &t : vec) {
        strm << t << ", ";
    }
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

