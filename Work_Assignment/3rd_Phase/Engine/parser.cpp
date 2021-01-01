#include "parser.h"
#include "Point.h"

int readPointsFile(string filename, vector<Point*> *points){
    string l, aux, token;
    ifstream file(filename);
    int i = 0, j;
    vector<float> tokens;

    if(!file.is_open()){
        cout << "Impossível abrir ficheiro: " << filename << "." << endl;
        return -1;
    }

    else{
        while(!file.eof()){
            getline(file, l);
            stringstream ss(l.c_str());

            if(l.c_str() != nullptr){
                for(j = 0; getline(ss, token, ',') && j < 3; j++){
                    tokens.push_back(stof(token));
                }

                Point *p = new Point(tokens[i++], tokens[i++], tokens[i++]);
                points -> push_back(p);
            }
        }
        points -> pop_back();
        file.close();
    }

    return 0;
}

Group* loadXMLFile(string filename){
    Group* group = nullptr;
    XMLDocument xmlDoc;
    XMLNode *pNode;
    XMLElement *pElement;
    string fileDir = "../../XMLs/" + filename;
    XMLError error = xmlDoc.LoadFile(fileDir.c_str());

    if(error == XML_SUCCESS){
        pNode = xmlDoc.FirstChild();

        if(pNode != nullptr){
            group = new Group();
            pElement = pNode -> FirstChildElement("group");
            parseGroup(group, pElement);
        }
    }

    else{
        cout << "Impossível abrir ficheiro: " << filename << "." << endl;
    }

    return group;
}

void parseTranslate(Group *group, XMLElement *element){
    float x = 0, y = 0, z = 0;

    if(element -> Attribute("time")){
        bool d = false;
        vector<Point*> points;

        if(element -> Attribute("derivative"))
            d = stoi(element -> Attribute("derivative")) == 1;

        float time = stof(element -> Attribute("time"));
        time = 1 / (time * 1000);
        element = element -> FirstChildElement("point");

        while (element != nullptr){
            if(element->Attribute("X"))
                x = stof(element->Attribute("X"));

            if(element->Attribute("Y"))
                y = stof(element->Attribute("Y"));

            if(element->Attribute("Z"))
                z = stof(element->Attribute("Z"));

            points.push_back(new Point (x, y, z));
            element = element -> NextSiblingElement("point");
        }

        group -> addTransformation(new Transformation(time, points, d, "translateTime"));
    }

    else {
        if (element -> Attribute("X"))
            x = stof(element->Attribute("X"));

        if (element -> Attribute("Y"))
            y = stof(element->Attribute("Y"));

        if (element -> Attribute("Z"))
            z = stof(element->Attribute("Z"));

        group -> addTransformation(new Transformation("translate", 0, x, y, z));
    }
}

void parseScale(Group *group, XMLElement *element){
    float x = 1, y = 1, z = 1;
    string type = "scale";

    if(element -> Attribute("X"))
        x = stof(element -> Attribute("X"));

    if(element -> Attribute("Y"))
        y = stof(element -> Attribute("Y"));

    if(element -> Attribute("Z"))
        z = stof(element -> Attribute("Z"));

    group -> addTransformation(new Transformation(type, 0, x, y, z));
}

void parseRotate(Group *group, XMLElement *element){
    float angle = 0, x = 0, y = 0, z = 0;
    string type = "rotate";

    if(element -> Attribute("time")){
        float time = stof(element -> Attribute("time"));
        angle = 360 / (time * 1000);
        type = "rotateTime";
    }

    else if(element -> Attribute("angle"))
        angle = stof(element -> Attribute("angle"));

    if(element -> Attribute("X"))
        x = stof(element -> Attribute("X"));

    if(element -> Attribute("Y"))
        y = stof(element -> Attribute("Y"));

    if(element -> Attribute("Z"))
        z = stof(element -> Attribute("Z"));

    group -> addTransformation(new Transformation(type, angle, x, y, z));
}

void parseModels(Group *group, XMLElement *element){
    string file;
    vector<Shape*> shapes;

    element = element -> FirstChildElement("model");

    if(element == nullptr) {
        cout << "Erro no XML: Modelo não disponível.";
        return;
    }

    while(element != nullptr){
        file = element -> Attribute("file");
        string fileDir = "../../Files3d/" + file;

        if(!file.empty()){
            vector<Point*> points;
            readPointsFile(fileDir, &points);

            if(points.size()){
                Shape *shape = new Shape(points);
                shapes.push_back(shape);
            }
        }

        element = element -> NextSiblingElement("model");
    }

    if(shapes.size())
        group -> setShapes(shapes);
}

void parseColour(Group *group, XMLElement *element) {
    float x = 1, y = 1, z = 1;
    string type = "colour";

    if (element->Attribute("R"))
        x = stof(element->Attribute("R"));

    if (element->Attribute("G"))
        y = stof(element->Attribute("G"));

    if (element->Attribute("B"))
        z = stof(element->Attribute("B"));

    group->addTransformation(new Transformation(type, 0, x, y, z));
}

void parseGroup(Group *group, XMLElement *gElement){
    XMLElement *element = gElement -> FirstChildElement();

    while (element){
        if (strcmp(element -> Name(),"translate") == 0)
            parseTranslate(group,element);

        else if (strcmp(element -> Name(),"scale") == 0)
            parseScale(group,element);

        else if (strcmp(element -> Name(),"rotate") == 0)
            parseRotate(group,element);

        else if (strcmp(element -> Name(),"models") == 0)
            parseModels(group, element);

        else if (strcmp(element -> Name(),"colour") == 0)
            parseColour(group, element);

        else if (strcmp(element -> Name(),"group") == 0){
            Group *child = new Group();
            group -> addGroup(child);
            parseGroup(child,element);
        }

        element = element -> NextSiblingElement();
    }
}