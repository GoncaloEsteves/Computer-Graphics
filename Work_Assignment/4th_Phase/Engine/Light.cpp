#include <vector>

#include "Light.h"

Light :: Light(){

}

Light :: Light(float *inf, vector<int> atr){
    info = inf;
    attributes = atr;
}

void Light :: apply(GLenum number){

    // posicao da luz
    glLightfv(GL_LIGHT0 + number, GL_POSITION, info);

    // atributos da luz
    for (const int atr : attributes){
        switch(atr){
            case DIFFUSE:
                glLightfv(GL_LIGHT0 + number, GL_DIFFUSE, info + 4);
                break;

            case AMBIENT:
                glLightfv(GL_LIGHT0 + number, GL_AMBIENT, info + 8);
                break;

            case SPECULAR:
                glLightfv(GL_LIGHT0 + number, GL_SPECULAR, info + 12);
                break;

            default:
                break;
        }
    }
}