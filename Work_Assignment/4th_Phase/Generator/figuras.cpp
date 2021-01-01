#include "figuras.h"
#include <math.h>

using namespace std;

float quadrants[8][3] = {
        { 1,  1,  1},
        { 1,  1, -1},
        {-1,  1, -1},
        {-1,  1,  1},
        { 1, -1,  1},
        { 1, -1, -1},
        {-1, -1, -1},
        {-1, -1,  1},
};


float normals[6][3] = {
        {0,1,0},
        {0,-1,0},
        {0,0,1},
        {0,0,-1},
        {1,0,0},
        {-1,0,0}
};

float texturePlane[4][2] = {
        {1,0},
        {1,1},
        {0,1},
        {0,0}
};

Point makePoint(float radius, float angle, float height){
    Point p;

    p.x = radius * sin(angle);
    p.y = height;
    p.z = radius * cos(angle);

    return p;
}

Point makePointSphere (float radius, float beta, float alpha){
    Point p;

    p.x = radius * sin(alpha) * cos(beta);
    p.y = radius * sin(beta);
    p.z = radius * cos(beta) * cos(alpha);

    return p;
}

Point makePointTorus(float radiusIn,float radiusOut,float beta,float alpha){
    Point p;

    p.x = cos(alpha) * (radiusIn*cos(beta) + radiusOut);
    p.y = sin(alpha) * (radiusIn*cos(beta) + radiusOut);
    p.z = radiusIn  * sin(beta);

    return p;
}

Point makeNormalPoint(float angle, float teta){
    Point p;

    p.x = cos(angle) * sin(teta);
    p.y = sin(angle);
    p.z = cos(angle) * cos(teta);

    return p;
}

vector<Point> generatePlane(float x, float z, vector<Point> *normal, vector<float> *texture){
    vector<Point> points;
    Point p;
    int face[6] = { 0, 1, 3, 3, 1, 2 };
    int aux;

    for(int i = 0; i < 6; i++){
        aux = face[i];
        p.x = x * quadrants[aux][0];
        p.y = 0;
        p.z = z * quadrants[aux][2];
        points.push_back(p);

        p.x = 0;
        p.y = 1;
        p.z = 0;
        (*normal).push_back(p);

        (*texture).push_back(texturePlane[aux][0]);
        (*texture).push_back(texturePlane[aux][0]);
    }

    return points;
}

void generateBoxSides(float x, float y, float z, int divis, vector<Point>* points, vector<Point> *normal, vector<float> *texture){
    float stepz = z / (float)(divis); // Step X axis.
    float stepy = y / (float)(divis); // Step Y axis.
    float texZ_inv = 0.750000,texZ = 0.000000 , texY = 0.666667;
    float divTexZ = 0.250000 / (float)(divis) , divTexY = 0.333333 / (float)(divis); 
    float x2 = x/2;
    Point p1, p2, p3, p4, p5, p6;

    for (float i = y/2; i > -y/2; i -= stepy) {
        for (float j = -z/2; j < z/2; j += stepz) {
            /*
            * p4---p1
            *  |   |
            * p3---p2
            * plano da direita
            */
            //faces laterais 4,3,2 \ 2,1,4 || -1,-2,-3 \ -3 -4 -1
            p1.x = x2; p1.y = i; p1.z = j;

            p2.x = x2; p2.y = (i - stepy); p2.z = j;

            p3.x = x2; p3.y = (i - stepy); p3.z = (j + stepz);

            p4.x = x2; p4.y = i; p4.z = (j + stepz);

            //valores das normais
            p5.x = 1.000000;  p5.y = 0.000000; p5.z = 0.000000;
            p6.x = -1.000000;  p6.y = 0.000000; p6.z = 0.000000;

            points -> push_back(p4);
            points -> push_back(p3);
            points -> push_back(p2);

            points -> push_back(p2);
            points -> push_back(p1);
            points -> push_back(p4);

            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);

            (*texture).push_back(texZ_inv-divTexZ); (*texture).push_back(texY);           // p4 tex
            (*texture).push_back(texZ_inv-divTexZ); (*texture).push_back(texY-divTexY);   // p3 tex
            (*texture).push_back(texZ_inv); (*texture).push_back(texY-divTexY);           // p2 tex
            (*texture).push_back(texZ_inv); (*texture).push_back(texY-divTexY);           // p2 tex 
            (*texture).push_back(texZ_inv); (*texture).push_back(texY);                   // p1 tex
            (*texture).push_back(texZ_inv-divTexZ); (*texture).push_back(texY);           // p4 tex

            p1.x = -x2;

            p2.x = -x2;

            p3.x = -x2;

            p4.x = -x2;

            points -> push_back(p1);
            points -> push_back(p2);
            points -> push_back(p3);

            points -> push_back(p3);
            points -> push_back(p4);
            points -> push_back(p1);

            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);

            (*texture).push_back(texZ); (*texture).push_back(texY);                    // p1 tex
            (*texture).push_back(texZ); (*texture).push_back(texY-divTexY);            // p2 tex
            (*texture).push_back(texZ+divTexZ); (*texture).push_back(texY-divTexY);    // p3 tex
            (*texture).push_back(texZ+divTexZ); (*texture).push_back(texY-divTexY);    // p3 tex
            (*texture).push_back(texZ+divTexZ); (*texture).push_back(texY);            // p4 tex
            (*texture).push_back(texZ); (*texture).push_back(texY);                    // p1 tex

            //Variacao da divisão horizontal de textura 
            texZ_inv-= divTexZ;
            texZ+=divTexZ;
        }
        //decremento da divisão vertical de textura 
        texY-= divTexY;
        //reset para a posição de textura horizontal inicial 
        texZ = 0.000000;
        texZ_inv = 0.750000;
    }
}


void generateBoxTops(float x, float y, float z, int divis, vector<Point>* points, vector<Point> *normal, vector<float> *texture){
    float stepx = x / (float)(divis); // Step X axis.
    float stepz = z / (float)(divis); // Step Y axis.
    float texX_inv = 0.500000, texX = 0.500000 , texZTop = 1.000000, texZBot = 0.333333;
    float divTexX = 0.250000 / (float)(divis); float divTexZ = 0.333333 / (float)(divis); 
    float y2 = y/2;
    Point p1, p2, p3, p4, p5, p6;

    for (float i = z/2; i > -z/2; i -= stepz) {
        for (float j = -x / 2; j < x / 2; j += stepx) {
            /* * p2---p3
            *     | \ |
            *    p1---p4
            *
            * plano do topo
            */
            //faces laterais 4,3,2 \ 2,1,4 || -1,-2,-3 \ -3 -4 -1
            p1.x = j; p1.y = y2; p1.z = i;

            p2.x = j; p2.y = y2; p2.z = (i - stepz);

            p3.x = (j + stepx); p3.y = y2; p3.z = (i - stepz);

            p4.x = (j + stepx); p4.y = y2; p4.z = i;

            //valores das normais
            p5.x = 0.000000;  p5.y = 1.000000; p5.z = 0.000000;
            p6.x = 0.000000;  p6.y = -1.000000; p6.z = 0.000000;

            points -> push_back(p4);
            points -> push_back(p3);
            points -> push_back(p2);

            points -> push_back(p2);
            points -> push_back(p1);
            points -> push_back(p4);

            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);

            (*texture).push_back(texX_inv-divTexX); (*texture).push_back(texZTop);           // p4 tex
            (*texture).push_back(texX_inv-divTexX); (*texture).push_back(texZTop-divTexZ);   // p3 tex
            (*texture).push_back(texX_inv); (*texture).push_back(texZTop-divTexZ);           // p2 tex
            (*texture).push_back(texX_inv); (*texture).push_back(texZTop-divTexZ);           // p2 tex 
            (*texture).push_back(texX_inv); (*texture).push_back(texZTop);                   // p1 tex
            (*texture).push_back(texX_inv-divTexX); (*texture).push_back(texZTop);           // p4 tex

            p1.y = -y2;

            p2.y = -y2;

            p3.y = -y2;

            p4.y = -y2;

            points -> push_back(p1);
            points -> push_back(p2);
            points -> push_back(p3);

            points -> push_back(p3);
            points -> push_back(p4);
            points -> push_back(p1);

            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);

            (*texture).push_back(texX); (*texture).push_back(texZBot);                    // p1 tex
            (*texture).push_back(texX); (*texture).push_back(texZBot-divTexZ);            // p2 tex
            (*texture).push_back(texX-divTexX); (*texture).push_back(texZBot-divTexZ);    // p3 tex
            (*texture).push_back(texX-divTexX); (*texture).push_back(texZBot-divTexZ);    // p3 tex
            (*texture).push_back(texX-divTexX); (*texture).push_back(texZBot);            // p4 tex
            (*texture).push_back(texX); (*texture).push_back(texZBot);                    // p1 tex

            //Variacao da divisão horizontal de textura 
            texX_inv-= divTexX;
            texX-= divTexX;
        }
        //decremento da divisão vertical de textura 
        texZTop-= divTexZ;
        texZBot-= divTexZ;
        //reset para a posição de textura horizontal inicial 
        texX_inv = 0.500000;
        texX = 0.500000;
    }
}


void generateBoxFront(float x, float y, float z, int divis, vector<Point>* points, vector<Point> *normal, vector<float> *texture){
    float stepx = x / (float)(divis); // Step X axis.
    float stepy = y / (float)(divis); // Step Y axis.
    float texX_inv = 1.000000, texX = 0.250000 , texY = 0.666667;
    float divTexX = 0.250000 / (float)(divis) , divTexY = 0.333333 / (float)(divis); 
    float z2 = z/2;
    Point p1, p2, p3, p4, p5, p6;

    for (float i = y/2; i > -y/2; i -= stepy) {
        for (float j = -x/2; j < x/2; j += stepx) {
            /*
            * p1---p4
            *  | \ |
            * p2---p3
            * plano frontal
            */
            //faces frontais 1,2,3 \ 3,4,1 || -4 -3 -2 \ -2 -1 -4
            p1.x = j; p1.y = i; p1.z = z2;

            p2.x = j; p2.y = (i - stepy); p2.z = z2;

            p3.x = (j + stepx); p3.y = (i - stepy); p3.z = z2;

            p4.x = (j + stepx); p4.y = i; p4.z = z2;

            //valores das normais
            p5.x = 0.000000;  p5.y = 0.000000; p5.z = 1.000000;
            p6.x = 0.000000;  p6.y = 0.000000; p6.z = -1.000000;

            points -> push_back(p1);
            points -> push_back(p2);
            points -> push_back(p3);

            points -> push_back(p3);
            points -> push_back(p4);
            points -> push_back(p1);

            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);
            (*normal).push_back(p5);

            (*texture).push_back(texX); (*texture).push_back(texY);                    // p1 tex
            (*texture).push_back(texX); (*texture).push_back(texY-divTexY);             // p2 tex
            (*texture).push_back(texX+divTexX); (*texture).push_back(texY-divTexY);      // p3 tex
            (*texture).push_back(texX+divTexX); (*texture).push_back(texY-divTexY);      // p3 tex
            (*texture).push_back(texX+divTexX); (*texture).push_back(texY);             // p4 tex
            (*texture).push_back(texX); (*texture).push_back(texY);                    // p1 tex

            p1.z = -z2;

            p2.z = -z2;

            p3.z = -z2;

            p4.z = -z2;

            points -> push_back(p4);
            points -> push_back(p3);
            points -> push_back(p2);

            points -> push_back(p2);
            points -> push_back(p1);
            points -> push_back(p4);

            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);
            (*normal).push_back(p6);

            (*texture).push_back(texX_inv-divTexX); (*texture).push_back(texY);           // p4 tex
            (*texture).push_back(texX_inv-divTexX); (*texture).push_back(texY-divTexY);    // p3 tex
            (*texture).push_back(texX_inv); (*texture).push_back(texY-divTexY);           // p2 tex
            (*texture).push_back(texX_inv); (*texture).push_back(texY-divTexY);           // p2 tex 
            (*texture).push_back(texX_inv); (*texture).push_back(texY);                  // p1 tex
            (*texture).push_back(texX_inv-divTexX); (*texture).push_back(texY);           // p4 tex

            //Variacao da divisão horizontal de textura 
            texX+= divTexX;
            texX_inv-= divTexX;
        }
        //decremento da divisão vertical de textura 
        texY-= divTexY;
        //reset para a posição de textura horizontal inicial 
        texX = 0.250000;
        texX_inv = 1.000000;
    }
}

vector<Point> generateBox(float x, float y, float z, int divis, vector<Point> *normal, vector<float> *texture){
    vector<Point> points;

    generateBoxSides(x,y,z,divis, &points, normal, texture);
    generateBoxTops(x,y,z,divis, &points, normal, texture);
    generateBoxFront(x,y,z,divis, &points, normal, texture);

    return points;
}

vector<Point> generateSphere(float radius, int slices, int stacks, vector<Point> *normal, vector<float> *texture){
    float alpha1, alpha2, beta1, beta2;
    vector<Point> points;
    Point p1, p2, p3, p4;

    //ciclo das stacks (variância do beta por stack de baixo para cima da esfera)
    for (int i = 0; i < stacks; i++){
        // incremento dos betas
        beta1 = (float)i * (float)(M_PI / stacks) - (float)M_PI_2;
        beta2 = (float)(i + 1) * (float)(M_PI / stacks) - (float)M_PI_2;
        //ciclo das slices (variância do alpha por slice à volta da esfera)
        for (int j = 0; j < slices; j++) {
            //incremento dos alphas
            alpha1 = (float)j * 2 * (float)M_PI / (float)slices;
            alpha2 = (float)(j + 1) * 2 * (float)M_PI / (float)slices;

            //geração dos pontos dos triângulos através das coordenadas esféricas alpha1/2 e beta1/2
            p1 = makePointSphere(radius, beta2, alpha1);
            p2 = makePointSphere(radius, beta1, alpha1);
            p3 = makePointSphere(radius, beta2, alpha2);
            p4 = makePointSphere(radius, beta1, alpha2);

            //primeiro triangulo
            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            //segundo triangulo
            points.push_back(p3);
            points.push_back(p2);
            points.push_back(p4);

            p1 = makePointSphere(1, beta2, alpha1);
            p2 = makePointSphere(1, beta1, alpha1);
            p3 = makePointSphere(1, beta2, alpha2);
            p4 = makePointSphere(1, beta1, alpha2);

            //normal
            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p3);
            (*normal).push_back(p3);
            (*normal).push_back(p2);
            (*normal).push_back(p4);

            //texture
            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)(i+1) / (float)stacks);
            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)i / (float)stacks);
            (*texture).push_back((float)(j+1) / (float)slices);
            (*texture).push_back((float)(i+1) / (float)stacks);
            (*texture).push_back((float)(j+1) / (float)slices);
            (*texture).push_back((float)(i+1) / (float)stacks);
            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)i / (float)stacks);
            (*texture).push_back((float)(j+1) / (float)slices);
            (*texture).push_back((float)i / (float)stacks);
        }
    }

    return points;
}

vector<Point> generateCone(float radius, float height, int slices, int stacks, vector<Point> *normal, vector<float> *texture){
    float alpha, scaleH, scaleR, radiusNow, radiusNext, heightNow, heightNext, teta, tetaNext, angle, res, resNext;
    vector<Point> points;
    Point p1, p2, p3, p4;

    alpha = (float)(2 * M_PI) / (float)slices;
    scaleH = height / (float)stacks;
    scaleR = radius / (float)stacks;

    p1.x = 0;
    p1.y = 0;
    p1.z = 0;

    p4.x = 0;
    p4.y = -1;
    p4.z = 0;

    //base do cone
    for (int i=0; i < slices; i++){
        teta = (float)i * alpha;
        tetaNext = (float)(i+1) * alpha;

        p2 = makePoint(radius, tetaNext, 0);
        p3 = makePoint(radius, teta, 0);

        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        //normal
        (*normal).push_back(p4);
        (*normal).push_back(p4);
        (*normal).push_back(p4);

        //texture
        (*texture).push_back(0.25f);
        (*texture).push_back(0.5f);
        (*texture).push_back(0.25f + cos(tetaNext) / 4.0f);
        (*texture).push_back(0.5f + sin(tetaNext) / 2.0f);
        (*texture).push_back(0.25f + cos(teta) / 4.0f);
        (*texture).push_back(0.5f + sin(teta) / 2.0f);
    }

    angle = atan(radius/height);

    //corpo do cone
    for (int i = 0; i < stacks; i++){
        heightNow = (float)i * scaleH;
        heightNext = (float)(i + 1) * scaleH;
        radiusNow = radius - (float)i * scaleR;
        radiusNext = radius - (float)(i + 1) * scaleR;

        for (int j = 0; j < slices; j++) {
            teta = (float)j * alpha;
            tetaNext = (float)(j + 1) * alpha;

            p1 = makePoint(radiusNow, teta, heightNow);
            p2 = makePoint(radiusNow, tetaNext, heightNow);
            p3 = makePoint(radiusNext, tetaNext, heightNext);
            p4 = makePoint(radiusNext, teta, heightNext);

            //primeiro triangulo
            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            //segundo triangulo
            points.push_back(p1);
            points.push_back(p3);
            points.push_back(p4);

            //normal
            p1 = makeNormalPoint(angle,teta);
            p2 = makeNormalPoint(angle,tetaNext);

            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p2);
            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p1);

            //texture
            res = (float)(stacks - i) / (float)stacks;
            resNext = (float)(stacks - (i+1)) / (float)stacks;

            (*texture).push_back(0.75f + 0.25f * cos(teta) * res);
            (*texture).push_back(0.5f  +  0.5f * sin(teta) * res);
            (*texture).push_back(0.75f + 0.25f * cos(tetaNext) * res);
            (*texture).push_back( 0.5f +  0.5f * sin(tetaNext) * res);
            (*texture).push_back(0.75f + 0.25f * cos(tetaNext) * resNext);
            (*texture).push_back( 0.5f +  0.5f * sin(tetaNext) * resNext);
            (*texture).push_back(0.75f + 0.25f * cos(teta) * res);
            (*texture).push_back( 0.5f +  0.5f * sin(teta) * res);
            (*texture).push_back(0.75f + 0.25f * cos(tetaNext) * resNext);
            (*texture).push_back( 0.5f +  0.5f * sin(tetaNext) * resNext);
            (*texture).push_back(0.75f + 0.25f * cos(teta) * resNext);
            (*texture).push_back( 0.5f +  0.5f * sin(teta) * resNext);
        }
    }

    return points;
}

vector<Point> generateCylinder (float radius, float height, int slices, int stacks, vector<Point> *normal, vector<float> *texture){
    float alpha, teta, tetaNext, scaleHeigth, heigthNow, heigthNext;
    vector<Point> points;
    Point p1, p2, p3, p4, p5, p6, p7;

    alpha = (float)(2 * M_PI) / (float)slices;
    scaleHeigth = height / (float)stacks;

    p1.x = 0;
    p1.z = 0;
    p4.x = 0;
    p4.z = 0;

    for(int i = 0; i < slices; i++){
        teta = (float)i * alpha;
        tetaNext = (float)(i + 1) * alpha;

        //base
        p1.y = -(height / 2);
        p4.y = -1;
        p2 = makePoint(radius, tetaNext, -(height / 2));
        p3 = makePoint(radius, teta, -(height / 2));

        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        //normal
        (*normal).push_back(p4);
        (*normal).push_back(p4);
        (*normal).push_back(p4);

        //texture
        (*texture).push_back(0.8125f);
        (*texture).push_back(0.1875f);
        (*texture).push_back(0.8125f + 0.1875f * sin(teta + alpha));
        (*texture).push_back(0.1875f + 0.1875f * cos(teta + alpha));
        (*texture).push_back(0.8125f + 0.1875f * sin(teta));
        (*texture).push_back(0.1875f + 0.1875f * cos(teta));

        //topo
        p1.y = (height / 2);
        p4.y = 1;
        p2 = makePoint(radius, teta, (height / 2));
        p3 = makePoint(radius, tetaNext, (height / 2));

        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        //normal
        (*normal).push_back(p3);
        (*normal).push_back(p3);
        (*normal).push_back(p3);

        //texture
        (*texture).push_back(0.4375f);
        (*texture).push_back(0.1875f);
        (*texture).push_back(0.4375f + 0.1875f * sin(teta));
        (*texture).push_back(0.1875f + 0.1875f * cos(teta));
        (*texture).push_back(0.4375f + 0.1875f * sin(tetaNext));
        (*texture).push_back(0.1875f + 0.1875f * cos(tetaNext));
    }

    for (int i = 0; i < stacks; i++) {

        heigthNow = -(height / 2) + ((float)i * scaleHeigth);
        heigthNext = heigthNow + scaleHeigth;

        for (int j = 0; j < slices; j++) {
            teta = (float)j * alpha;
            tetaNext = (float)(j + 1) * alpha;

            p1 = makePoint(radius, teta, heigthNow);
            p2 = makePoint(radius, tetaNext, heigthNow);
            p3 = makePoint(radius, tetaNext, heigthNext);
            p4 = makePoint(radius, teta, heigthNext);

            //primeiro triangulo
            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            //segundo triangulo
            points.push_back(p1);
            points.push_back(p3);
            points.push_back(p4);

            p5.x = sin(teta);
            p5.y = 0;
            p5.z = cos(teta);
            p6.x = sin(tetaNext);
            p6.z = cos(teta);
            p7.x = sin(tetaNext);
            p7.y = 0;
            p7.z = cos(tetaNext);

            //normal
            (*normal).push_back(p5);
            (*normal).push_back(p6);
            (*normal).push_back(p5);
            (*normal).push_back(p7);
            (*normal).push_back(p7);
            (*normal).push_back(p7);

            //texture
            (*texture).push_back((1.0f / (float)slices) * ((float)j));
            (*texture).push_back((float)i*0.625f / (float)stacks + 0.375f);

            (*texture).push_back((1.0f / (float)slices) * ((float)j + 1));
            (*texture).push_back((float)i*0.625f / (float)stacks + 0.375f);

            (*texture).push_back((1.0f / (float)slices) * (float)(j + 1));
            (*texture).push_back((float)(i+1)*0.625f / (float)stacks + 0.375f);

            (*texture).push_back((1.0f / (float)slices) * ((float)j));
            (*texture).push_back((float)i*0.625f / (float)stacks + 0.375f);

            (*texture).push_back((1.0f / (float)slices) * (float)(j + 1));
            (*texture).push_back((float)(i+1)*0.625f / (float)stacks + 0.375f);

            (*texture).push_back((1.0f / (float)slices) * (float)j);
            (*texture).push_back((float)(i+1)*0.625f / (float)stacks + 0.375f);
        }
    }

    return points;
}

vector<Point> generateTorus(float radiusIn,float radiusOut , int slices, int layers, vector<Point> *normal, vector<float> *texture){
    float alpha, nextAlpha, beta, nextBeta;
    Point p1, p2, p3, p4;
    vector<Point> points;

    for (int i = 0; i < layers; i++) {
        beta = (float)i * (float)(2 * M_PI / layers);
        nextBeta = (float)(i + 1) * (float)(2 * M_PI / layers);

        for (int j = 0; j < slices; j++) {
            alpha = (float)j * (float)(2 * M_PI / slices);
            nextAlpha = (float)(j + 1) * (float)(2 * M_PI / slices);

            p1 = makePointTorus(radiusIn, radiusOut, beta, alpha);
            p2 = makePointTorus(radiusIn, radiusOut, beta, nextAlpha);
            p3 = makePointTorus(radiusIn, radiusOut, nextBeta, alpha);
            p4 = makePointTorus(radiusIn, radiusOut, nextBeta, nextAlpha);

            //primeiro triangulo
            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            //segundo triangulo
            points.push_back(p3);
            points.push_back(p2);
            points.push_back(p4);

            p1 = makePointTorus(1, 1, beta, alpha);
            p2 = makePointTorus(1, 1, beta, nextAlpha);
            p3 = makePointTorus(1, 1, nextBeta, alpha);
            p4 = makePointTorus(1, 1, nextBeta, nextAlpha);

            //normal
            (*normal).push_back(p1);
            (*normal).push_back(p2);
            (*normal).push_back(p3);

            (*normal).push_back(p3);
            (*normal).push_back(p2);
            (*normal).push_back(p4);

            //textures
            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)i / (float)layers);

            (*texture).push_back((float)(j+1) / (float)slices);
            (*texture).push_back((float)i / (float)layers);

            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)(i+1) / (float)layers);

            (*texture).push_back((float)j / (float)slices);
            (*texture).push_back((float)(i+1) / (float)layers);

            (*texture).push_back((float)(j+1) / (float)slices);
            (*texture).push_back((float)i / (float)layers);

            (*texture).push_back((float)(j+1) / (float)slices);
            (*texture).push_back((float)(i+1) / (float)layers);
        }
    }

    return points;
}
