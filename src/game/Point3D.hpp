
#ifndef POINT3D_HPP
#define POINT3D_HPP

#include "Point2D.hpp"

namespace MCServer {

struct Point3D : public Point2D {
    Point3D()
    :z(0){}

    Point3D(Coordinate x, Coordinate y, Coordinate z)
    :Point2D(x, y), z(z) {}

    Point3D(const Point2D &p2d)
    :Point2D(p2d), z(0) {}

    int z;
};

inline constexpr bool operator==(const Point3D &x, const Point3D &y) {
    return x.x == y.x && x.y == y.y && x.z == y.z;
}

inline constexpr bool operator!=(const Point3D &x, const Point3D &y) {
    return !(x == y);
}

inline constexpr bool operator<(const Point3D &x, const Point3D &y) {
    return x.x < y.x ? true : (x.y < y.y ? true : (x.z < y.z));
}

inline constexpr bool operator>(const Point3D &x, const Point3D &y) {
    return x.x > y.x ? true : (x.y > y.y ? true : (x.z > y.z));
}

inline constexpr bool operator>=(const Point3D &x, const Point3D &y) {
    return !(x < y);
}

inline constexpr bool operator<=(const Point3D &x, const Point3D &y) {
    return !(x > y);
}

}

#endif

