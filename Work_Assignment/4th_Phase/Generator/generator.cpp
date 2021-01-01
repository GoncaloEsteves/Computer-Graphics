#include "generator.h"
#include "figuras.h"
#include "Patch.h"
#include "Point.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <string.h>

using namespace std;

void generatePointsFile(string filename, vector<Point> points, vector<Point> normals, vector<float> textures){
    ofstream file;

    string fileDir = "../../Files3d/" + filename;
    file.open(fileDir, ios_base::trunc);

    if (!file.is_open()) {
        cout << "Erro na abertura do ficheiro " << filename << endl;
    }
    else {
        file << points.size() << "\n";
        for (const Point point : points) {
            string x, y, z;

            x = to_string(point.x).append(",");
            y = to_string(point.y).append(",");
            z = to_string(point.z).append("\n");

            file << x << y << z;
        }

        file << normals.size() << "\n";
        for(const Point normal : normals){
            string x, y, z;
            x = to_string(normal.x).append(",");
            y = to_string(normal.y).append(",");
            z = to_string(normal.z).append("\n");

            file << x << y << z;
        }

        file << textures.size() / 2 << "\n";
        for(int i = 0; i < textures.size(); i+=2){
            string t1,t2;
            t1 = to_string(textures[i]).append(",");
            t2 = to_string(textures[i+1]).append("\n");

            file << t1 << t2;
        }

        file.close();
        cout << filename << " - Ficheiro criado!" << endl;
    }
}

int main (int argc, char **argv) {

    if (argc < 2){
        printf("Argumentos de entrada insuficientes!\n");
        return -1;
    }

    string file;
    vector<Point> points;
    vector<Point> normal;
    vector<float> texture;

    if (strcmp("plane", argv[1]) == 0 && argc == 5){
        file = argv[4];
        points = generatePlane(stof(argv[2]), stof(argv[3]), &normal, &texture);
    }
    else if (strcmp("box", argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateBox(stof(argv[2]), stof(argv[3]), stof(argv[4]),stoi(argv[5]), &normal, &texture);
    }
    else if (strcmp("sphere",argv[1]) == 0 && argc == 6){
        file = argv[5];
        points = generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), &normal, &texture);
    }
    else if (strcmp("cone",argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]), &normal, &texture);
    }
    else if (strcmp("cylinder",argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateCylinder(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]), &normal, &texture);
    }
    else if (strcmp("torus", argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateTorus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]), &normal, &texture);
    }
    else if (strcmp("-patch", argv[1]) == 0 && argc == 5){
        file = argv[4];
        Patch *p = new Patch(atoi(argv[3]), argv[2]);
        p -> BezierModelGenerator(&points,&normal,&texture);
    }
    else{
        printf("Input inválido!\n");
        return -1;
    }

    if(points.size() > 0)
        generatePointsFile(file, points, normal, texture);

    return 0;
}