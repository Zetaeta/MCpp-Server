
#include <map>

#include "World.hpp"
#include "Coordinates.hpp"
#include "Chunk.hpp"

using std::string;
using std::map;

namespace MCServer {

struct WorldData {
    string name;
    map<Point, Chunk> chunks;
};

World::World(const string &name, uint128_t uid) {
    m->name = name;
}

Chunk & World::chunkAt(int x, int y) {
    return chunkAt(Point(x, y));
}

Chunk & World::chunkAt(const Point &pt) {
    return m->chunks[pt];
}

}

