
#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <utility>

typedef int32_t Coordinate;

class Point : public std::pair<Coordinate, Coordinate> {
public:
    Point()
    :x(first), y(second)

    Point(const Coordinate &x, const Coordinate &y)
    :std::pair(x, y), x(first), y(second) {}

    int &x, &y;
};

#endif
