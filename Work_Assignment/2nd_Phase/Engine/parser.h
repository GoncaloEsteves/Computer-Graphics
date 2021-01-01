#ifndef ENGINE_PARSER_H
#define ENGINE_PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "tinyxml2.h"
#include "Point.h"
#include "Group.h"
#include "Shape.h"
#include <vector>

using namespace std;
using namespace tinyxml2;

Group* loadXMLFile(string filename, vector<Point*> *points);
void parseGroup(Group *group, XMLElement *gElement, vector<Point*> *orbits, int d);

#endif