
#ifndef POINT3D_HPP
#define POINT3D_HPP

#include "Point2D.hpp"

namespace MCServer {

struct BlockCoordinates;

struct Point3D : public Point2D {
    Point3D()
    :y(0){}

    Point3D(Coordinate x, Coordinate y, Coordinate z)
    :Point2D(x, z), y(y) {}

    Point3D(const Point3D &p)
    :Point3D(p.x, p.y, p.z) {}

    Point3D(const Point2D &p2d)
    :Point2D(p2d), y(0) {}

    Point3D(const BlockCoordinates &p);

    template<class T>
    Point3D(const std::initializer_list<T> &i)
    :Point3D(*i.begin(), *(i.begin() + 1), *(i.end() - 1)) {
        assert(i.size() == 3);
    }

    const Point3D operator=(const Point3D &p) {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    const Point3D operator=(const Point2D &p) {
        x = p.x;
        y = 0;
        z = p.z;
        return *this;
    }

    const Point3D operator=(const BlockCoordinates &p);

    double distanceTo3D(Point3D other);

    Coordinate y;
};

inline constexpr bool operator==(const Point3D &x, const Point3D &y) {
    return x.x == y.x && x.y == y.y && x.z == y.z;
}

inline constexpr bool operator!=(const Point3D &x, const Point3D &y) {
    return !(x == y);
}

inline constexpr bool operator<(const Point3D &x, const Point3D &y) {
    return x.x < y.x ? true : (x.z < y.z ? true : (x.y < y.y));
}

inline constexpr bool operator>(const Point3D &x, const Point3D &y) {
    return x.x > y.x ? true : (x.z > y.z ? true : (x.y > y.y));
}

inline constexpr bool operator>=(const Point3D &x, const Point3D &y) {
    return !(x < y);
}

inline constexpr bool operator<=(const Point3D &x, const Point3D &y) {
    return !(x > y);
}

template <typename T>
inline T & operator<<(T &stream, const Point3D &p) {
    stream << '{' << p.x << ", " << p.y << ", " << p.z << '}';
    return stream;
}

}

#endif

