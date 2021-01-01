#ifndef ENGINE_TRANSFORMATION_H
#define ENGINE_TRANSFORMATION_H

#include <string>

using namespace std;

class Transformation {

    private:
        string type;
        float angle, x, y, z;

    public:
        Transformation();
        Transformation(string t, float a, float xx, float yy, float zz);
        string getType();
        float getAngle();
        float getX();
        float getY();
        float getZ();
};

#endif