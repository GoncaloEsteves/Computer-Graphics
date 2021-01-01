#include "parser.h"
#include "Point.h"
#include "Material.h"
#include "Scene.h"

#define POINT 1
#define DIRECTIONAL 2
#define DIFFUSE 3
#define AMBIENT 4
#define SPECULAR 5

int readPointsFile(string filename, vector<Point*> *points, vector<Point*> *normalList, vector<float> *textureList){
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Unable to open file: " << filename << "." << endl; return -1;
    }

    else{
        int index, j;
        string token, l, t;
        vector<float> tokens;
        index = 0;
        getline(file, l);
        int numVertex = atoi(l.c_str());

        for(int i = 0; i < numVertex; i++){
            getline(file, l);
            stringstream ss(l);
            j = 0;

            while(j < 3 && getline(ss, token, ',')){
                tokens.push_back(stof(token));
                j++;
            }

            points -> push_back(new Point(tokens[index++], tokens[index++], tokens[index++]));
        }

        index = 0;
        getline(file, l);
        int numNormals = atoi(l.c_str());

        for(int i=0; i < numNormals; i++){
            getline(file, l);
            stringstream ss(l);
            j = 0;

            while(j < 3 && getline(ss, token,',')){
                tokens.push_back(stof(token));
                j++;
            }

            normalList -> push_back(new Point(tokens[index++],tokens[index++],tokens[index++]));
        }

        getline(file, l);
        int numTexture = atoi(l.c_str());

        for(int i=0; i < numTexture; i++){
            getline(file, l);
            stringstream ss(l);
            j = 0;

            while(j < 2 && getline(ss, token, ',')){
                j++;
                textureList -> push_back(stof(token));
            }
        }

        file.close();
    }
    return 0;
}

Scene* loadXMLFile(string filename){
    Scene* scene = nullptr;
    XMLDocument xmlDoc;
    XMLNode *pNode;
    XMLElement *pElement;
    string fileDir = "../../XMLs/" + filename;
    XMLError error = xmlDoc.LoadFile(fileDir.c_str());

    if(error == XML_SUCCESS){
        Group *group = new Group();
        scene = new Scene(group);
        pNode = xmlDoc.FirstChild();

        if(pNode != nullptr){
            pElement = pNode -> FirstChildElement("lights");

            if (pElement){
                pElement -> FirstChildElement();
                parseLights(scene, pElement);
            }

            pElement = pNode -> FirstChildElement("group");
            parseGroup(scene, group, pElement);
        }
    }

    else{
        cout << "Impossível abrir ficheiro: " << filename << "." << endl;
    }

    return scene;
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

void parseMaterial(Shape* shape, XMLElement* element){
    Transformation* diffuse = nullptr;
    Transformation* ambient = nullptr;
    Transformation* specular = new Transformation(0, 0, 0);
    Transformation* emission = new Transformation(0, 0, 0);


    // Difusa
    if(element -> Attribute("diffR") || element -> Attribute("diffG") || element -> Attribute("diffB")){
        diffuse = new Transformation(0.8f, 0.8f, 0.8f);

        if (element -> Attribute("diffR"))
            diffuse -> setX(stof(element -> Attribute("diffR")));
        if (element -> Attribute("diffG"))
            diffuse -> setY(stof(element -> Attribute("diffG")));
        if (element -> Attribute("diffB"))
            diffuse -> setZ(stof(element -> Attribute("diffB")));
    }

    // Ambiente
    if(element -> Attribute("ambR") || element -> Attribute("ambG") || element -> Attribute("ambB")) {
        ambient = new Transformation(0.2f, 0.2f, 0.2f);

        if (element -> Attribute("ambR"))
            ambient -> setX(stof(element -> Attribute("ambR")));
        if (element -> Attribute("ambG"))
            ambient -> setY(stof(element -> Attribute("ambG")));
        if (element -> Attribute("ambB"))
            ambient -> setZ(stof(element -> Attribute("ambB")));
    }


    // Especular
    if(element -> Attribute("specR"))
        specular -> setX(stof(element -> Attribute("specR")));
    if(element -> Attribute("specG"))
        specular -> setY(stof(element -> Attribute("specG")));
    if(element -> Attribute("specB"))
        specular -> setZ(stof(element -> Attribute("specB")));

    // Emissão
    if(element -> Attribute("emiR"))
        emission -> setX(stof(element -> Attribute("emiR")));
    if(element -> Attribute("emiG"))
        emission -> setY(stof(element -> Attribute("emiG")));
    if(element -> Attribute("emiB"))
        emission -> setZ(stof(element -> Attribute("emiB")));


    Material* m = new Material(diffuse, ambient, specular, emission);
    shape -> setParseMat(m);
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
            vector<Point*> normal;
            vector<float> texture;
            readPointsFile(fileDir, &points,&normal, &texture);

            if(points.size()){
                Shape *shape;

                if(element -> Attribute("texture"))
                    shape = new Shape(element -> Attribute("texture"), points, normal, texture);

                else
                    shape = new Shape(points, normal, texture);

                parseMaterial(shape, element);
                shapes.push_back(shape);
            }
        }

        element = element -> NextSiblingElement("model");
    }

    if(shapes.size())
        group -> setShapes(shapes);
}

void parseLights (Scene *scene, XMLElement *pElement){

    if ((pElement = pElement -> FirstChildElement("light")) == nullptr){
        cout << "Erro no XML: Não há iluminação definida." << endl;
        return;
    }

    while (pElement != nullptr){

        if (pElement -> Attribute("type")){

            int type = -1;
            float *info = (float *) calloc (16, sizeof(float));
            vector<int> attributes;

            if (strcmp(pElement -> Attribute("type"), "POINT") == 0){
                type = POINT;
                info[3] = 1.0f;
            }
            else if (strcmp(pElement -> Attribute("type"), "DIRECTIONAL") == 0){
                type = DIRECTIONAL;
                info[3] = 0.0f;
            }

            // posicao da luz
            if (pElement -> Attribute("posX")){
                info[0] = stof(pElement -> Attribute("posX"));
            }
            if (pElement -> Attribute("posY")){
                info[1] = stof(pElement -> Attribute("posY"));
            }
            if (pElement -> Attribute("posZ")){
                info[2] = stof(pElement -> Attribute("posZ"));
            }

            // luz difusa
            if (pElement -> Attribute("diffR") || pElement -> Attribute("diffG") || pElement -> Attribute("diffB")){
                attributes.push_back(DIFFUSE);

                if (pElement -> Attribute("diffR"))
                    info[4] = stof(pElement -> Attribute("diffR"));
                if (pElement -> Attribute("diffG"))
                    info[5] = stof(pElement -> Attribute("diffG"));
                if (pElement -> Attribute("diffB"))
                    info[6] = stof(pElement -> Attribute("diffB"));

                info[7] = 1.0f;
            }

            // luz ambiente
            if (pElement -> Attribute("ambR") || pElement -> Attribute("ambG") || pElement -> Attribute("ambB")){
                attributes.push_back(AMBIENT);

                if (pElement -> Attribute("ambR"))
                    info[8] = stof(pElement -> Attribute("ambR"));
                if (pElement -> Attribute("ambG"))
                    info[9] = stof(pElement -> Attribute("ambG"));
                if (pElement -> Attribute("ambB"))
                    info[10] = stof(pElement -> Attribute("ambB"));

                info[11] = 1.0f;
            }

            // luz especular
            if (pElement -> Attribute("specR") || pElement -> Attribute("specG") || pElement -> Attribute("specB")){
                attributes.push_back(SPECULAR);

                if (pElement -> Attribute("specR"))
                    info[12] = stof(pElement -> Attribute("specR"));
                if (pElement -> Attribute("specG"))
                    info[13] = stof(pElement -> Attribute("specG"));
                if (pElement -> Attribute("specB"))
                    info[14] = stof(pElement -> Attribute("specB"));

                info[15] = 1.0f;
            }

            if (type != -1){
                Light *light = new Light(info,attributes);
                scene -> addLight(light);
            }
            else
                free(info);
        }
        pElement = pElement -> NextSiblingElement("light");
    }
}

void parseGroup(Scene *scene, Group *group, XMLElement *gElement){
    XMLElement *element = gElement -> FirstChildElement();

    while (element){
        if (strcmp(element -> Name(),"translate") == 0)
            parseTranslate(group, element);

        else if (strcmp(element -> Name(),"scale") == 0)
            parseScale(group, element);

        else if (strcmp(element -> Name(),"rotate") == 0)
            parseRotate(group, element);

        else if (strcmp(element -> Name(),"models") == 0)
            parseModels(group, element);

        else if (strcmp(element -> Name(),"group") == 0){
            Group *child = new Group();
            group -> addGroup(child);
            parseGroup(scene, child, element);
        }

        element = element -> NextSiblingElement();
    }
}