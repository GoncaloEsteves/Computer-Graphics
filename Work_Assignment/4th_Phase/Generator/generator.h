#ifndef GENERATOR_GENERATOR_H
#define GENERATOR_GENERATOR_H

#include "figuras.h"
#include <vector>
#include <string>

void generatePointsFile(string filename, vector<Point> points, vector<Point> normals, vector<float> textures);

#endif