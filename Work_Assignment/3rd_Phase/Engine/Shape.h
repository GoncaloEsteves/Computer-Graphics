#ifndef ENGINE_SHAPE_H
#define ENGINE_SHAPE_H

#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include "Point.h"
#endif


using namespace std;

class Shape {
    private:
        int numVert;
        GLuint buffer[1];

    public:
        Shape();
        Shape(vector<Point*> p);
        void prepareBuffer(vector<Point*> p);
        void draw();
};

#endif