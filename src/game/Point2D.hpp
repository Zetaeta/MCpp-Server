
#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <assert.h>
#include <initializer_list>

#include "Coordinate.hpp"

namespace MCServer {

struct Point2D {
    constexpr Point2D()
    :x(0), z(0) {}

    constexpr Point2D(Coordinate x, Coordinate z)
    :x(x), z(z) {
    }

    // Works with initialiser_list of anything convertible to Coordinate.
    template<class T>
    Point2D(const std::initializer_list<T> &i)
    :Point2D(*i.begin(), *(i.end() - 1)) {
        assert(i.size() == 2);
    }

    const Point2D operator=(const Point2D &p) {
        x = p.x;
        z = p.z;
        return *this;
    }

    Coordinate x, z;
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

