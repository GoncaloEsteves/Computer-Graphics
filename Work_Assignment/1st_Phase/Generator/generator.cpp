#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <string.h>
using namespace std;

void generatePlane(float x, float z, string filename){
    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::trunc);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }
    else{
        file << -(x / 2) << "," << 0.0 << "," << (z / 2)  << "\n";
        file << (x / 2) << "," << 0.0<< "," << -(z / 2)  << "\n";
        file << -(x / 2) << "," << 0.0 << "," << -(z / 2)  << "\n";

        file << -(x / 2) << "," << 0.0 << "," << (z / 2)  << "\n";
        file << (x / 2) << "," << 0.0 << "," << (z / 2)  << "\n";
        file << (x / 2) << "," << 0.0 << "," << -(z / 2)  << "\n";

        file.close();
        cout << filename << " -> ficheiro criado com sucesso!" << endl;
    }
}

void generateBoxSides(float x, float y, float z, int divis, string filename){
    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::trunc);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }
    else{
        float stepz = z / (float)(divis); // Step X axis.
        float stepy = y / (float)(divis); // Step Y axis.
        float x2 = x/2;

        for (float i = y/2; i > -y/2; i -= stepy) {
            for (float j = -z/2; j < z/2; j += stepz) {
                /*
                 * p4---p1
                 *  |   |
                 * p3---p2
                 * Right plane points.
                 */
                //faces laterais 4,3,2 \ 2,1,4 || -1,-2,-3 \ -3 -4 -1
                file << x2 << "," << i << "," << (j+stepz) << "\n";                //p4
                file << x2 << "," << (i-stepy) << "," << (j+stepz) << "\n";        //p3
                file << x2 << "," << (i-stepy) << "," << j << "\n";                //p2

                file << x2 << "," << (i-stepy) << "," << j << "\n";                //p2
                file << x2 << "," << i << "," << j << "\n";                        //p1
                file << x2 << "," << i << "," << (j+stepz) << "\n";                //p4

                file << -x2 << "," << i << "," << j << "\n";                        //-p1
                file << -x2 << "," << (i-stepy) << "," << j << "\n";                //-p2
                file << -x2 << "," << (i-stepy) << "," << (j+stepz) << "\n";        //-p3

                file << -x2 << "," << (i-stepy) << "," << (j+stepz) << "\n";        //-p3
                file << -x2 << "," << i << "," << (j+stepz) << "\n";                //-p4
                file << -x2 << "," << i << "," << j << "\n";                        //-p1
            }
        }
        file.close();
        cout << filename << " -> Pontos das laterais do Cubo criados com sucesso!" << endl;
    }
}

void generateBoxTops(float x, float y, float z, int divis, string filename){
    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::app);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }
    else{
        float stepx = x / (float)(divis); // Step X axis.
        float stepz = z / (float)(divis); // Step Y axis.
        float y2 = y/2;

        for (float i = z/2; i > -z/2; i -= stepz) {
            for (float j = -x / 2; j < x / 2; j += stepx) {
                /* * p2---p3
                *     | \ |
                *    p1---p4
                *
                * Top plane points.
                */
                //faces laterais 4,3,2 \ 2,1,4 || -1,-2,-3 \ -3 -4 -1
                file << j+stepx << "," << y2 << "," << i << "\n";                 //p4
                file << j+stepx << "," << y2 << "," << (i-stepz) << "\n";         //p3
                file << j << "," << y2 << "," << (i-stepz) << "\n";               //p2

                file << j << "," << y2 << "," << (i-stepz) << "\n";               //p2
                file << j << "," << y2 << "," << i << "\n";                       //p1
                file << j+stepx << "," << y2 << "," << i << "\n";                 //p4

                file << j << "," << -y2 << "," << i << "\n";                      //-p1
                file << j << "," << -y2 << "," << (i-stepz) << "\n";              //-p2
                file << j+stepx << "," << -y2 << "," << (i-stepz) << "\n";        //-p3

                file << j+stepx << "," << -y2 << "," << (i-stepz) << "\n";        //-p3
                file << j+stepx << "," << -y2 << "," << i << "\n";                //-p4
                file << j << "," << -y2 << "," << i << "\n";                      //-p1
            }
        }
        file.close();
        cout << filename << " -> Pontos da Base/Topo do Cubo criados com sucesso!" << endl;
    }
}

void generateBoxFront(float x, float y, float z, int divis, string filename){
    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::app);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }
    else{
        float stepx = x / (float)(divis); // Step X axis.
        float stepy = y / (float)(divis); // Step Y axis.
        float z2 = z/2;

        for (float i = y/2; i > -y/2; i -= stepy) {
            for (float j = -x/2; j < x/2; j += stepx) {
                /*
                 * p1---p4
                 *  | \ |
                 * p2---p3
                 * Front plane points.
                 */
                //faces frontais 1,2,3 \ 3,4,1 || -4 -3 -2 \ -2 -1 -4
                file << j << "," << i << "," << z2 << "\n";                        //p1
                file << j << "," << (i-stepy) << "," << z2 << "\n";                //p2
                file << (j+stepx) << "," << (i-stepy) << "," << z2 << "\n";        //p3

                file << (j+stepx) << "," << (i-stepy) << "," << z2 << "\n";        //p3
                file << (j+stepx) << "," << i << "," << z2 << "\n";                //p4
                file << j << "," << i << "," << z2 << "\n";                        //p1
                //---
                file << (j+stepx) << "," << i << "," << -z2 << "\n";               //-p4
                file << (j+stepx) << "," << (i-stepy) << "," << -z2 << "\n";       //-p3
                file << j << "," << (i-stepy) << "," << -z2 << "\n";               //-p2

                file << j << "," << (i-stepy) << "," << -z2 << "\n";               //-p2
                file << j << "," << i << "," << -z2 << "\n";                       //-p1
                file << (j+stepx) << "," << i << "," << -z2 << "\n";               //-p4
            }
        }
        file.close();
        cout << filename << " -> Pontos das frentes do Cubo criados com sucesso!!\n\n -> Ficheiro criado com sucesso!!" << endl;
    }
}

void generateBox(float x, float y, float z, int divis, string filename){
    generateBoxSides(x,y,z,divis,filename);
    generateBoxTops(x,y,z,divis,filename);
    generateBoxFront(x,y,z,divis,filename);
}

void generateSphere(float radius, int slices, int stacks, string filename) {
    float alpha1, alpha2, beta1, beta2;
    float sliceAngle, stackAngle;

    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::trunc);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }

    else {
        //escrita do numero de pontos da esfera na 1ª linha
        //file << slices*stacks*6 << "\n";

        //ciclo das stacks (variância do beta por stack de baixo para cima da esfera)
        for (int i = 0; i < stacks; i++){
            // incremento dos betas
            beta1 = i * (M_PI / stacks) - M_PI_2;
            beta2 = (i + 1) * (M_PI / stacks) - M_PI_2;
            //ciclo das slices (variância do alpha por slice à volta da esfera)
            for (int j = 0; j < slices; j++) {
                //incremento dos alphas
                alpha1 = j * 2 * M_PI / slices;
                alpha2 = (j + 1) * 2 * M_PI / slices;
                //geração dos pontos dos triângulos através das coordenadas esféricas alpha1/2 e beta1/2
                file << radius * cos(beta1)*sin(alpha1) << "," << radius * sin(beta1) << "," << radius * cos(beta1)*cos(alpha1) << "\n";
                file << radius * cos(beta1)*sin(alpha2) << "," << radius * sin(beta1) << "," << radius * cos(beta1)*cos(alpha2) << "\n";
                file << radius * cos(beta2)*sin(alpha2) << "," << radius * sin(beta2) << "," << radius * cos(beta2)*cos(alpha2) << "\n";

                file << radius * cos(beta1)*sin(alpha1) << "," << radius * sin(beta1) << "," <<  radius * cos(beta1)*cos(alpha1) << "\n";
                file << radius * cos(beta2)*sin(alpha2) << "," << radius * sin(beta2) << "," <<  radius * cos(beta2)*cos(alpha2) << "\n";
                file << radius * cos(beta2)*sin(alpha1) << "," << radius * sin(beta2) << "," <<  radius * cos(beta2)*cos(alpha1) << "\n";
            }
        }
        file.close();
        cout << filename << " -> ficheiro criado com sucesso!" << endl;
    }
}

void generateCone(float radius, float height, int slices, int stacks, string filename) {
    float alpha, scaleH, scaleR, radiusNow, radiusNext, heightNow, heightNext, teta, tetaNext;

    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::trunc);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }
    else {
        //escrita do numero de pontos da esfera na 1ª linha
        //file << slices*stacks...

        alpha = (2 * M_PI) / slices;
        scaleH = height/stacks;
        scaleR = radius/stacks;

        //BASE DO CONE
        for (int i=0; i<slices;i++){
            teta = i * alpha;
            tetaNext = (i+1) * alpha;


            file << radius * sin(tetaNext) << "," << 0.0 << "," << radius * cos(tetaNext) << "\n";
            file << radius * sin(teta) << "," << 0.0 << "," << radius * cos(teta) << "\n";
            file << 0.0 << "," << 0.0 << "," << 0.0 << "\n";
        }

        //CORPO DO CONE
        for (int i = 0; i < stacks; i++){
            heightNow = (float)i*scaleH;
            heightNext = heightNow + scaleH;
            radiusNow = radius - (float)i * scaleR;
            radiusNext = radius - (float)(i + 1) * scaleR;
            for (int j = 0; j < slices; j++) {
                teta = (float)j * alpha;
                tetaNext = (float)(j + 1) * alpha;

                file << radiusNow * sin(teta) << "," << heightNow << "," << radiusNow * cos(teta) << "\n";
                file << radiusNow * sin(tetaNext) << "," << heightNow << "," << radiusNow * cos(tetaNext) << "\n";
                file << radiusNext * sin(tetaNext) << "," << heightNext << "," << radiusNext * cos(tetaNext) << "\n";

                file << radiusNow * sin(teta) << "," << heightNow << "," << radiusNow * cos(teta) << "\n";
                file << radiusNext * sin(tetaNext) << "," << heightNext << "," << radiusNext * cos(tetaNext) << "\n";
                file << radiusNext * sin(teta) << "," << heightNext << "," << radiusNext * cos(teta) << "\n";
            }
        }
        file.close();
        cout << filename << " -> ficheiro criado com sucesso!" << endl;
    }
}

void generateCylinder (float radius, float height, int slices, int stacks, string filename){
    float alpha, teta, tetaNext, scaleHeigth, heigthNow, heigthNext, heightBase, heightTopo;

    ofstream file;
    string fileDir = "../../XMLs/" + filename;
    file.open(fileDir, ios_base::trunc);
    if (!file.is_open()) {
        cout << "Erro a abrir o ficheiro... " << filename << endl;
    }
    else {
        alpha = (2 * M_PI) / slices;
        scaleHeigth = height / stacks;

        for (int i = 0; i < stacks; i++) {

            heigthNow = -(height/2)+ (i * scaleHeigth);
            heigthNext = heigthNow + scaleHeigth;

            for (int j = 0; j < slices; j++) {
                teta = j * alpha;
                tetaNext = (j + 1) * alpha;

                //corpo do cilindro
                file << radius * sin(teta) << "," << heigthNow << "," << radius * cos(teta) << "\n";
                file << radius * sin(tetaNext) << "," << heigthNow << "," << radius * cos(tetaNext) << "\n";
                file << radius * sin(tetaNext) << "," << heigthNext << "," << radius * cos(tetaNext) << "\n";

                file << radius * sin(teta) << "," << heigthNow << "," << radius * cos(teta) << "\n";
                file << radius * sin(tetaNext) << "," << heigthNext << "," << radius * cos(tetaNext) << "\n";
                file << radius * sin(teta) << "," << heigthNext << "," << radius * cos(teta) << "\n";

                //bases
                file << 0.0 << "," << -height / 2 << "," << 0.0 << "\n";
                file << radius * sin(tetaNext) << "," << -height / 2 << "," << radius * cos(tetaNext) << "\n";
                file << radius * sin(teta) << "," << -height / 2 << "," << radius * cos(teta) << "\n";

                file << 0.0 << "," << height / 2 << "," << 0.0 << "\n";
                file << radius * sin(teta) << "," << height / 2 << "," << radius * cos(teta) << "\n";
                file << radius * sin(tetaNext) << "," << height / 2 << "," << radius * cos(tetaNext) << "\n";
            }
        }
        file.close();
        cout << filename << " -> ficheiro criado com sucesso!" << endl;
    }
}

int main (int argc, char **argv)
{
        if (argc < 2)
    {
        printf("Argumentos de entrada insuficientes!\n");
        return -1;
    }
    if (strcmp("plane", argv[1]) == 0 && argc == 5)
    {
        generatePlane(stof(argv[2]),stof(argv[3]),argv[4]);
    }
    else if (strcmp("box", argv[1]) == 0 && argc == 7)
    {
        generateBox(stof(argv[2]), stof(argv[3]), stof(argv[4]),stoi(argv[5]), argv[6]);
    }
    else if (strcmp("sphere",argv[1]) == 0 && argc == 6)
    {
        generateSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), argv[5]);
    }
    else if (strcmp("cone",argv[1]) == 0 && argc == 7)
    {
        generateCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]),argv[6]);
    }
    else if (strcmp("cylinder",argv[1]) == 0 && argc == 7)
    {
        generateCylinder(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi (argv[5]),argv[6]);
    }
    else{
        printf("Input inválido!\n");
        return -1;
    }
    string file;
    return 0;
}
