
#ifndef CHUNKCOORDINATES_HPP
#define CHUNKCOORDINATES_HPP

#include "Point2D.hpp"

struct ChunkCoordinates : public Point2D {
    ChunkCoordinates()
    :Point2D(0, 0), x(0), y(0){}

    ChunkCoordinates(Coordinate x, Coordinate y)
    :Point2D(

    int x, y;
};

#endif
