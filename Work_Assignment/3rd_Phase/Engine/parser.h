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

using namespace std;
using namespace tinyxml2;

Group* loadXMLFile(string filename);
void parseGroup(Group *group, XMLElement *gElement);

#endif