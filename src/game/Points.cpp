
#include <math.h>

#include "Point3D.hpp"
#include "BlockCoordinates.hpp"
#include "ChunkCoordinates.hpp"

namespace MCServer {

double Point3D::distanceTo3D(Point3D other) {
    double x = other.x - this->x;
    double y = other.y - this->y;
    double z = other.z - this->z;
    double sum = (x * x) + (y * y) + (z * z);
    return sqrt(sum);
}

double BlockCoordinates::distanceTo3D(BlockCoordinates other) {
    double x = other.x - this->x;
    double y = other.y - this->y;
    double z = other.z - this->z;
    double sum = (x * x) + (y * y) + (z * z);
    return sqrt(sum);
}

BlockCoordinates::BlockCoordinates(const Point2D &p)
:x(p.x), y(0), z(p.z) {}

BlockCoordinates::BlockCoordinates(const Point3D &p)
:x(p.x), y(p.y), z(p.z) {}

BlockCoordinates::BlockCoordinates(const ChunkCoordinates &c)
:x(c.x * 16), y(0), z(c.z * 16) {}

const BlockCoordinates BlockCoordinates::operator=(const Point2D &p) {
    x = p.x;
    y = 0;
    z = p.z;
    return *this;
}

const BlockCoordinates BlockCoordinates::operator=(const Point3D &p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

const BlockCoordinates BlockCoordinates::operator=(const ChunkCoordinates &c) {
    x = c.x * 16;
    y = 0;
    z = c.z * 16;
    return *this;
}

Point3D::Point3D(const BlockCoordinates &p)
:Point3D(p.x, p.y, p.z) {}

const Point3D Point3D::operator=(const BlockCoordinates &p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return *this;
}

}

