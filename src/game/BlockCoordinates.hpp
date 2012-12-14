
#ifndef BLOCKCOORDINATES_HPP
#define BLOCKCOORDINATES_HPP

#include <initializer_list>

namespace MCServer {

struct Point3D;
struct Point2D;
struct ChunkCoordinates;

struct BlockCoordinates {
    BlockCoordinates()
    :x(0), y(0), z(0){}

    BlockCoordinates(unsigned x, unsigned y, unsigned z)
    :x(x), y(y), z(z) {}

    BlockCoordinates(const BlockCoordinates &p)
    :BlockCoordinates(p.x, p.y, p.z) {}

    template<class T>
    BlockCoordinates(const std::initializer_list<T> &i)
    :BlockCoordinates(*i.begin(), *(i.begin() + 1), *(i.end() - 1)) {
        assert(i.size() == 3);
    }
    
    BlockCoordinates(const Point2D &p);

    BlockCoordinates(const Point3D &p);

    BlockCoordinates(const ChunkCoordinates &p);

    const BlockCoordinates operator=(const Point2D &p);

    const BlockCoordinates operator=(const Point3D &p);

    const BlockCoordinates operator=(const ChunkCoordinates &p);

    const BlockCoordinates operator=(const BlockCoordinates &p) {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }


    double distanceTo3D(BlockCoordinates other);

    unsigned int x, y, z;
};

inline constexpr bool operator==(const BlockCoordinates &x, const BlockCoordinates &y) {
    return x.x == y.x && x.y == y.y && x.z == y.z;
}

inline constexpr bool operator!=(const BlockCoordinates &x, const BlockCoordinates &y) {
    return !(x == y);
}

inline constexpr bool operator<(const BlockCoordinates &x, const BlockCoordinates &y) {
    return x.x < y.x ? true : (x.z < y.z ? true : (x.y < y.y));
}

inline constexpr bool operator>(const BlockCoordinates &x, const BlockCoordinates &y) {
    return x.x > y.x ? true : (x.z > y.z ? true : (x.y > y.y));
}

inline constexpr bool operator>=(const BlockCoordinates &x, const BlockCoordinates &y) {
    return !(x < y);
}

inline constexpr bool operator<=(const BlockCoordinates &x, const BlockCoordinates &y) {
    return !(x > y);
}

template <typename T>
inline T & operator<<(T &stream, const BlockCoordinates &p) {
    stream << '{' << p.x << ", " << p.y << ", " << p.z << '}';
    return stream;
}

}

#endif

