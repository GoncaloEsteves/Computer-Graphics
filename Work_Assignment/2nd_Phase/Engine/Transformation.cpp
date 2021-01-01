#include "Transformation.h"

Transformation :: Transformation(){
}

Transformation ::Transformation(string t, float a, float xx, float yy, float zz) {
    type = t;
    angle = a;
    x = xx;
    y = yy;
    z = zz;
}

string Transformation :: getType() {
    return type;
}

float Transformation :: getAngle() {
    return angle;
}

float Transformation :: getX() {
    return x;
}

float Transformation :: getY() {
    return y;
}

float Transformation :: getZ() {
    return z;
}