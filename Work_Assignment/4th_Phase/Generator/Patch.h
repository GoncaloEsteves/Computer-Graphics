#ifndef GENERATOR_PATCH_H
#define GENERATOR_PATCH_H

#include "Point.h"
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

class Patch {
    private:
        int nPatches, nPoints, tesselation;
        vector<Point> controlPoints;
        map<int, vector<int>> patches;

    public:
        float m[4][4] = {{-1.0f,  3.0f, -3.0f,  1.0f},
                         { 3.0f, -6.0f,  3.0f,  0.0f},
                         {-3.0f,  3.0f,  0.0f,  0.0f},
                         { 1.0f,  0.0f,  0.0f,  0.0f}};

        Patch();
        Patch(vector<Point> p);
        Patch(int tess, string file);
        void multMatrixVector(float *m, float *v, float *res);
        void parsePatchFile(string filename);
        void normalize(float *a);
        void cross(float *a, float *b, float *res);
        void getPatchPoints(int patch, vector<Point>* points, vector<float>* texture, vector<Point>* normal);
        float* getTangent(float tu, float tv, float mX[4][4], float mY[4][4], float mZ[4][4], int type);
        Point* getPoint(float ta, float tb, float coordX[4][4], float coordY[4][4], float coordZ[4][4]);
        void BezierModelGenerator(vector<Point> *vert, vector<Point> *normal, vector<float> *texture);
};

#endif