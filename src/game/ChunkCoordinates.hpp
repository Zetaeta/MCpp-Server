
#ifndef CHUNKCOORDINATES_HPP
#define CHUNKCOORDINATES_HPP

#include <initializer_list>

#include "Point2D.hpp"

namespace MCServer {

struct ChunkCoordinates {
    constexpr ChunkCoordinates()
    :ChunkCoordinates(0, 0) {}

    constexpr ChunkCoordinates(Coordinate x, Coordinate z)
    :x(x), z(z) {}

    constexpr ChunkCoordinates(const Point2D &p)
    :x(p.x / 16), z(p.z / 16) {}

    constexpr ChunkCoordinates(Point2D &&p)
    :x(p.x / 16), z(p.z / 16) {}

    ChunkCoordinates(const std::initializer_list<int> &i)
    :ChunkCoordinates(*i.begin(), *(i.end() - 1)) {
        assert(i.size() == 2);
    }

    constexpr operator Point2D() const {
        return Point2D(x * 16, z *  16);
    }

    Coordinate x, z;
    constexpr Coordinate blockX() const {
        return x * 16;
    }
    constexpr Coordinate blockZ() const {
        return z * 16;
    }
};

}

#endif
