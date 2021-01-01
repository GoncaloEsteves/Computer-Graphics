#ifndef ENGINE_POINT_H
#define ENGINE_POINT_H

#include <math.h>
#include <string>

class Point {
    private:
        float x, y, z;

    public:
        Point();
        Point(float xx, float yy, float zz);
        float getX();
        float getY();
        float getZ();
};

#endif