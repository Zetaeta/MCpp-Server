
#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <assert.h>
#include <initializer_list>

#include "Coordinate.hpp"

namespace MCServer {

struct Point2D {
    constexpr Point2D()
    :x(0), z(0) {}

    constexpr Point2D(Coordinate x, Coordinate y)
    :x(y), z(y) {
    }

    Point2D(const std::initializer_list<int> &i)
    :Point2D(*i.begin(), *(i.end() - 1)) {
        assert(i.size() == 2);
    }

    int x, z;

};

inline constexpr bool operator==(const Point2D &x, const Point2D &y) {
    return x.x == y.x && x.z == y.z;
}

inline constexpr bool operator!=(const Point2D &x, const Point2D &y) {
    return !(x == y);
}

inline constexpr bool operator<(const Point2D &x, const Point2D &y) {
    return x.x < y.x ? true : (x.z < y.z);
}

inline constexpr bool operator>(const Point2D &x, const Point2D &y) {
    return x.x > y.x ? true : (x.z > y.z);
}

inline constexpr bool operator>=(const Point2D &x, const Point2D &y) {
    return !(x < y);
}

inline constexpr bool operator<=(const Point2D &x, const Point2D &y) {
    return !(x > y);
}

}

#endif

