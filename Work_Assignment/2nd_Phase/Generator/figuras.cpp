#include "figuras.h"
#include <math.h>

using namespace std;

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

vector<Point> generatePlane(float x, float z){
    vector<Point> points;
    Point p1, p2, p3, p4;

    p1.x = -(x / 2); p1.y = 0.0; p1.z = (z / 2);

    p2.x = (x / 2); p2.y = 0.0; p2.z = -(z / 2);

    p3.x = -(x / 2); p3.y = 0.0; p3.z = -(z / 2);

    p4.x = (x / 2); p4.y = 0.0; p4.z = (z / 2);

    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);

    points.push_back(p1);
    points.push_back(p4);
    points.push_back(p2);

    return points;
}

void generateBoxSides(float x, float y, float z, int divis, vector<Point>* points){
    float stepz = z / (float)(divis); // Step X axis.
    float stepy = y / (float)(divis); // Step Y axis.
    float x2 = x/2;
    Point p1, p2, p3, p4;

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

            points -> push_back(p4);
            points -> push_back(p3);
            points -> push_back(p2);

            points -> push_back(p2);
            points -> push_back(p1);
            points -> push_back(p4);

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
        }
    }
}

void generateBoxTops(float x, float y, float z, int divis, vector<Point>* points){
    float stepx = x / (float)(divis); // Step X axis.
    float stepz = z / (float)(divis); // Step Y axis.
    float y2 = y/2;
    Point p1, p2, p3, p4;

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

            points -> push_back(p4);
            points -> push_back(p3);
            points -> push_back(p2);

            points -> push_back(p2);
            points -> push_back(p1);
            points -> push_back(p4);

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
        }
    }
}

void generateBoxFront(float x, float y, float z, int divis, vector<Point>* points){
    float stepx = x / (float)(divis); // Step X axis.
    float stepy = y / (float)(divis); // Step Y axis.
    float z2 = z/2;
    Point p1, p2, p3, p4;

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

            points -> push_back(p1);
            points -> push_back(p2);
            points -> push_back(p3);

            points -> push_back(p3);
            points -> push_back(p4);
            points -> push_back(p1);

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
        }
    }
}

vector<Point> generateBox(float x, float y, float z, int divis){
    vector<Point> points;

    generateBoxSides(x,y,z,divis, &points);
    generateBoxTops(x,y,z,divis, &points);
    generateBoxFront(x,y,z,divis, &points);

    return points;
}

vector<Point> generateSphere(float radius, int slices, int stacks) {
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
            p1 = makePointSphere(radius, beta1, alpha1);
            p2 = makePointSphere(radius, beta1, alpha2);
            p3 = makePointSphere(radius, beta2, alpha2);
            p4 = makePointSphere(radius, beta2, alpha1);

            //primeiro triangulo
            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            //segundo triangulo
            points.push_back(p1);
            points.push_back(p3);
            points.push_back(p4);
        }
    }

    return points;
}

vector<Point> generateCone(float radius, float height, int slices, int stacks) {
    float alpha, scaleH, scaleR, radiusNow, radiusNext, heightNow, heightNext, teta, tetaNext;
    vector<Point> points;
    Point p1, p2, p3, p4;

    alpha = (float)(2 * M_PI) / (float)slices;
    scaleH = height / (float)stacks;
    scaleR = radius / (float)stacks;

    //base do cone
    p1.x = 0;
    p1.y = 0;
    p1.z = 0;

    for (int i=0; i < slices; i++){
        teta = (float)i * alpha;
        tetaNext = (float)(i+1) * alpha;

        p2 = makePoint(radius, tetaNext, 0);
        p3 = makePoint(radius, teta, 0);

        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
    }

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
        }
    }

    return points;
}

vector<Point> generateCylinder (float radius, float height, int slices, int stacks){
    float alpha, teta, tetaNext, scaleHeigth, heigthNow, heigthNext;
    vector<Point> points;
    Point p1, p2, p3, p4;

    alpha = (float)(2 * M_PI) / (float)slices;
    scaleHeigth = height / (float)stacks;

    p1.x = 0;
    p1.z = 0;

    for(int i = 0; i < slices; i++){
        teta = (float)i * alpha;
        tetaNext = (float)(i + 1) * alpha;

        p1.y = -(height / 2);
        p2 = makePoint(radius, tetaNext, -(height / 2));
        p3 = makePoint(radius, teta, -(height / 2));

        //base
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);

        p1.y = (height / 2);
        p2 = makePoint(radius, teta, (height / 2));
        p3 = makePoint(radius, tetaNext, (height / 2));

        //topo
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
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
        }
    }

    return points;
}

vector<Point> generateTorus(float radiusIn,float radiusOut , int slices, int layers){
    float alpha, nextAlpha, beta, nextBeta;
    Point p1, p2, p3, p4;
    vector<Point> points;

    for (int i = 0; i < layers; i++) {
        beta = (float)i * (float)(2 * M_PI / layers);
        nextBeta = (float)(i + 1) * (float)(2 * M_PI / layers);

        for (int j = 0; j < slices; j++) {
            alpha = (float)j * (float)(2 * M_PI / slices);
            nextAlpha = (float)(j + 1) * (float)(2 * M_PI / slices);

            p1 = makePointTorus(radiusIn, radiusOut, nextBeta, alpha);
            p2 = makePointTorus(radiusIn, radiusOut, beta, alpha);
            p3 = makePointTorus(radiusIn, radiusOut, nextBeta, nextAlpha);
            p4 = makePointTorus(radiusIn, radiusOut, beta, nextAlpha);

            //primeiro triangulo
            points.push_back(p1);
            points.push_back(p2);
            points.push_back(p3);

            //segundo triangulo
            points.push_back(p3);
            points.push_back(p2);
            points.push_back(p4);
        }
    }

    return points;
}