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
#include "Light.h"
#include "Scene.h"

using namespace std;
using namespace tinyxml2;

Scene* loadXMLFile(string filename);
void parseGroup(Scene *scene, Group *group, XMLElement *gElement);
void parseLights (Scene *scene, XMLElement *pElement);
void parseMaterial(Shape* shape, XMLElement* element);

#endif