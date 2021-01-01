#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include <vector>

#include "Point.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


#define POINT 1
#define DIRECTIONAL 2
#define DIFFUSE 3
#define AMBIENT 4
#define SPECULAR 5

using namespace std;

class Light {
    private:
        float *info;
        vector<int> attributes;

    public:
        Light();
        Light(float *inf, vector<int> atr);
        void apply(GLenum number);
};

#endif