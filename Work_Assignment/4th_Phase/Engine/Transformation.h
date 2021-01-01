#ifndef ENGINE_TRANSFORMATION_H
#define ENGINE_TRANSFORMATION_H

#include <string>
#include <vector>

#include "Point.h"

using namespace std;

class Transformation {

    private:
        string type;
        float angle, x, y, z, time;
        vector<Point*> controlPoints, pointsCurve;
        bool deriv;
        float vetor[4] = {1, 0, 0};


    public:
        Transformation();
        Transformation(string t, float a, float xx, float yy, float zz);
        Transformation(float ti, vector<Point*> p, bool d, string t);
        Transformation(float r, float g, float b);
        string getType();
        float getAngle();
        float getX();
        float getY();
        float getZ();
        float getTime();
        bool getDeriv();
        float* getVetor();
        vector<Point*> getPointsCurve();
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void normalize(float *a);
        void cross(float *a, float *b, float *res);
        void rotMatrix(float *r, float *xx, float *yy, float *zz);
        void multMatrixVector(float *m, float *v, float *res);
        void getCatmullRomPoint(float t, int *indexes, float *p, float *der);
        void getGlobalCatmullRomPoint(float gt, float *p, float *der);
        void setCatmullPoints();
};

#endif