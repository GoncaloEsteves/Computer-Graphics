#ifndef ENGINE_GROUP_H
#define ENGINE_GROUP_H

#include <vector>

#include "Transformation.h"
#include "Shape.h"

using namespace std;

class Group {
    private:
        vector<Group*> groups;
        vector<Transformation*> trans;
        vector<Shape*> shapes;

    public:
        Group();
        void addGroup(Group* g);
        void addTransformation(Transformation* t);
        void setShapes(vector<Shape*> sh);
        vector<Group*> getGroups();
        vector<Transformation*> getTrans();
        vector<Shape*> getShapes();
};

#endif