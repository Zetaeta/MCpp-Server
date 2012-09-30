
#ifndef CHUNKCOORDINATES_HPP
#define CHUNKCOORDINATES_HPP

#include <initializer_list>

#include "Point2D.hpp"

namespace MCServer {

struct ChunkCoordinates : public Point2D {
    ChunkCoordinates()
    :ChunkCoordinates(0, 0) {}

    ChunkCoordinates(Coordinate x, Coordinate y)
    :Point2D(x * 16, y * 16), blockX(Point2D::x), blockY(Point2D::y) {}

    ChunkCoordinates(const Point2D &p)
    :Point2D(p), blockX(Point2D::x), blockY(Point2D::y) {}

    ChunkCoordinates(Point2D &&p)
    :Point2D(p), blockX(Point2D::x), blockY(Point2D::y) {}

    ChunkCoordinates(const std::initializer_list<int> &i)
    :ChunkCoordinates(*i.begin(), *(i.end() - 1)) {
        assert(i.size() == 2);
    }

    int x, y;
    int &blockX, &blockY;
};

}

#endif
