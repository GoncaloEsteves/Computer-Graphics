#include <GL/glew.h>

#include "Material.h"

Material :: Material(){

}

Material :: Material(Transformation* d, Transformation* a, Transformation* s, Transformation* e){
    diffuse[3] = -1;
    ambient[3] = -1;

    if(d != nullptr) {
        diffuse[0] = d->getX();
        diffuse[1] = d->getY();
        diffuse[2] = d->getZ();
        diffuse[3] = 1;
    }

    if(a != nullptr) {
        ambient[0] = a->getX();
        ambient[1] = a->getY();
        ambient[2] = a->getZ();
        ambient[3] = 1;
    }

    specular[0] = s->getX();
    specular[1] = s->getY();
    specular[2] = s->getZ();
    specular[3] = 1;

    emission[0] = e->getX();
    emission[1] = e->getY();
    emission[2] = e->getZ();
    emission[3] = 1;
}

void Material :: draw() {

    if(diffuse[3] != -1)
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    if(ambient[3] != -1)
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}