#ifndef ENGINE_SHAPE_H
#define ENGINE_SHAPE_H

#include <vector>

#include "Point.h"
#include "Material.h"

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
        int numVert[3];
        GLuint buffer[3];
        GLuint texture;
        Material* materials;

    public:
        Shape();
        Shape(vector<Point*> vert, vector<Point*> normal, vector<float> text);
        Shape(string textFile, vector<Point*> vert, vector<Point*> normal, vector<float> text);
        void setParseMat(Material* c);
        void prepareBuffer(vector<Point*> vert, vector<Point*> normal, vector<float> text);
        void loadTexture(string textFile);
        void draw();
};

#endif