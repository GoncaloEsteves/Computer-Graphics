#include "Group.h"

Group :: Group(){
}

void Group :: addGroup(Group* g){
    groups.push_back(g);
}

void Group :: addTransformation(Transformation* t){
    trans.push_back(t);
}

void Group :: setShapes(vector<Shape*> sh){
    shapes = sh;
}

vector<Group*> Group :: getGroups(){
    return groups;
}

vector<Transformation*> Group :: getTrans(){
    return trans;
}

vector<Shape*> Group :: getShapes(){
    return shapes;
}