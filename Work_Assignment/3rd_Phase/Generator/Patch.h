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
        Patch();
        Patch(vector<Point> p);
        Patch(int tess, string file);
        void multMatrixVector(float *m, float *v, float *res);
        void parsePatchFile(string filename);
        Point* getPoint(float ta, float tb, float coordX[4][4], float coordY[4][4], float coordZ[4][4]);
        vector<Point> getPatchPoints(int patch);
        vector<Point> BezierModelGenerator();
};

#endif