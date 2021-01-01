#ifndef GENERATOR_POINT_H
#define GENERATOR_POINT_H

#include <math.h>
#include <string>

class Point {
    public:
        float x, y, z;
        Point();
        Point(float xx, float yy, float zz);
        float getX();
        float getY();
        float getZ();
};

#endif