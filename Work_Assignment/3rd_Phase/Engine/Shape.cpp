#include "Shape.h"

Shape :: Shape(){

}

Shape :: Shape(vector<Point*> p){
    numVert = p.size();
    prepareBuffer(p);
}

void Shape :: prepareBuffer(vector<Point *> p) {
    int i = 0;
    float* vs = new float[p.size() * 3];

    for(vector<Point*> :: const_iterator v_it = p.begin(); v_it != p.end(); ++v_it){
        vs[i++] = (*v_it) -> getX();
        vs[i++] = (*v_it) -> getY();
        vs[i++] = (*v_it) -> getZ();
    }

    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVert * 3, vs, GL_STATIC_DRAW);

    delete [] vs;
}

void Shape :: draw(){
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, numVert * 3);
}