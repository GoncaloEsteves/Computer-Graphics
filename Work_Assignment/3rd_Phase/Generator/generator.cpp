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

void generatePointsFile(string filename, vector<Point> points) {
    ofstream file;

    string fileDir = "../../Files3d/" + filename;
    file.open(fileDir, ios_base::trunc);

    if (!file.is_open()) {
        cout << "Erro na abertura do ficheiro " << filename << endl;
    }
    else {
        for (const Point point : points) {
            string x, y, z;

            x = to_string(point.x).append(",");
            y = to_string(point.y).append(",");
            z = to_string(point.z).append("\n");

            file << x << y << z;
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

    if (strcmp("plane", argv[1]) == 0 && argc == 5){
        file = argv[4];
        points = generatePlane(stof(argv[2]),stof(argv[3]));
    }
    else if (strcmp("box", argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateBox(stof(argv[2]), stof(argv[3]), stof(argv[4]),stoi(argv[5]));
    }
    else if (strcmp("sphere",argv[1]) == 0 && argc == 6){
        file = argv[5];
        points = generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]));
    }
    else if (strcmp("cone",argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]));
    }
    else if (strcmp("cylinder",argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateCylinder(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]));
    }
    else if (strcmp("torus", argv[1]) == 0 && argc == 7){
        file = argv[6];
        points = generateTorus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]));
    }
    else if (strcmp("-patch", argv[1]) == 0 && argc == 5){
        Patch *p = new Patch(atoi(argv[3]), argv[2]);
        vector<Point> pointsAux = p -> BezierModelGenerator();
        generatePointsFile(argv[4], pointsAux);
    }
    else{
        printf("Input inválido!\n");
        return -1;
    }

    if(points.size() > 0)
        generatePointsFile(file, points);

    return 0;
}