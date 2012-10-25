
#ifndef CHUNKCOORDINATES_HPP
#define CHUNKCOORDINATES_HPP

#include <initializer_list>
#include <vector>
#include <iostream>

#include "Point2D.hpp"

namespace MCServer {

struct ChunkCoordinates {
    constexpr ChunkCoordinates()
    :ChunkCoordinates(0, 0) {}

    constexpr ChunkCoordinates(int x, int z)
    :x(x), z(z) {}

    constexpr ChunkCoordinates(const Point2D &p)
    :x(p.x / 16), z(p.z / 16) {}

    constexpr ChunkCoordinates(const ChunkCoordinates &c)
    :x(c.x), z(c.z) {}

    constexpr ChunkCoordinates(Point2D &&p)
    :x(p.x / 16), z(p.z / 16) {}

    ChunkCoordinates(const std::initializer_list<int> &i)
    :ChunkCoordinates(*i.begin(), *(i.end() - 1)) {
        assert(i.size() == 2);
    }

    constexpr operator Point2D() const {
        return Point2D(x * 16, z *  16);
    }

    int x, z;
    constexpr Coordinate blockX() const {
        return x * 16;
    }
    constexpr Coordinate blockZ() const {
        return z * 16;
    }

    inline ChunkCoordinates operator=(const ChunkCoordinates &c) {
        x = c.x;
        z = c.z;
        return *this;
    }

    inline ChunkCoordinates operator=(const Point2D &p) {
        x = p.x / 16;
        z = p.z / 16;
        return *this;
    }

    template <class T>
    inline ChunkCoordinates operator=(const std::initializer_list<T> &i) {
        assert(i.size() == 2);
        x = *i.begin();
        z = *(i.end() - 1);
        return *this;
    }

    std::vector<ChunkCoordinates> getSurroundingChunks(int radius, bool includeSelf = true) {
        size_t diameter = radius * 2 + 1;
        std::vector<ChunkCoordinates> vec(diameter * diameter);
        int i = 0;
        for (int _x = x - radius; _x <= x + radius; ++_x) {
            for (int _z = z - radius; _z <= z + radius; ++_z) {
                if (!includeSelf && x == _x && z == _z)
                    continue;
                vec[i++] = {_x, _z};
            }
        }
        return vec;
    }
};

template <typename T>
inline T & operator<<(T& t, const ChunkCoordinates &c) {
    return t << '{' << c.x << ", " << c.z << '}';
}

inline constexpr bool operator==(const ChunkCoordinates &x, const ChunkCoordinates &y) {
    return x.x == y.x && x.z == y.z;
}

inline constexpr bool operator!=(const ChunkCoordinates &x, const ChunkCoordinates &y) {
    return !(x == y);
}

inline constexpr bool operator<(const ChunkCoordinates &x, const ChunkCoordinates &y) {
    return x.x < y.x || (!(y.x < x.x) &&(x.z < y.z));
}

inline constexpr bool operator>(const ChunkCoordinates &x, const ChunkCoordinates &y) {
    return x.x > y.x || (!(y.x > x.x) &&(x.z > y.z));
}

inline constexpr bool operator>=(const ChunkCoordinates &x, const ChunkCoordinates &y) {
    return !(x < y);
}

inline constexpr bool operator<=(const ChunkCoordinates &x, const ChunkCoordinates &y) {
    return !(x > y);
}

}

#endif
