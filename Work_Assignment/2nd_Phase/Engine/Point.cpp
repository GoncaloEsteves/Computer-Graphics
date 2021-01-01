#include "Point.h"

Point :: Point(){
    x = 0;
    y = 0;
    z = 0;
}

Point :: Point(float xx, float yy, float zz){
    x = xx;
    y = yy;
    z = zz;
}

float Point :: getX() {
    return x;
}

float Point :: getY() {
    return y;
}

float Point :: getZ() {
    return z;
}