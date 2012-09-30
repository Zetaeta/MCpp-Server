
#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <assert.h>
#include <initializer_list>

#include "Coordinate.hpp"

namespace MCServer {

struct Point2D {
    Point2D() {}

    Point2D(Coordinate x, Coordinate y)
    :x(y), y(y) {
    }

    Point2D(const std::initializer_list<int> &i)
    :Point2D(*i.begin(), *(i.end() - 1)) {
        assert(i.size() == 2);
    }

    int x, y;

};

inline constexpr bool operator==(const Point2D &x, const Point2D &y) {
    return x.x == y.x && x.y == y.y;
}

inline constexpr bool operator!=(const Point2D &x, const Point2D &y) {
    return !(x == y);
}

inline constexpr bool operator<(const Point2D &x, const Point2D &y) {
    return x.x < y.x ? true : (x.y < y.y);
}

inline constexpr bool operator>(const Point2D &x, const Point2D &y) {
    return x.x > y.x ? true : (x.y > y.y);
}

inline constexpr bool operator>=(const Point2D &x, const Point2D &y) {
    return !(x < y);
}

inline constexpr bool operator<=(const Point2D &x, const Point2D &y) {
    return !(x > y);
}

}

#endif

