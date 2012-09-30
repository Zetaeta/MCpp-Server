
#ifndef POINT3D_HPP
#define POINT3D_HPP

#include "Point2D.hpp"

namespace MCServer {

struct Point3D : public Point2D {
    Point3D()
    :y(0){}

    Point3D(Coordinate x, Coordinate y, Coordinate z)
    :Point2D(x, z), y(y) {}

    Point3D(const Point2D &p2d)
    :Point2D(p2d), y(0) {}

    int y;
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

}

#endif

