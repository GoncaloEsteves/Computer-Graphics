#ifndef FIGURAS_FIGURAS_H
#define FIGURAS_FIGURAS_H

#include <vector>

#include "Point.h"

using namespace std;

Point makePoint(float radius, float angle, float height);
Point makePointSphere (float radius, float beta, float alpha);
Point makePointTorus(float radiusIn,float radiusOut,float beta,float alpha);
vector<Point> generatePlane(float x, float z);
vector<Point> generateBox(float x, float y, float z, int div);
vector<Point> generateSphere(float radius, int slices, int stacks);
vector<Point> generateCone(float radius, float height, int slices, int stacks);
vector<Point> generateCylinder (float radius, float height, int slices, int stacks);
vector<Point> generateTorus(float radiusIn,float radiusOut , int slices, int layers);

#endif