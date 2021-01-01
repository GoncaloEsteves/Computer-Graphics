#include <IL/il.h>

#include "Shape.h"

Shape :: Shape(){

}

Shape :: Shape(vector<Point*> vert, vector<Point*> normal, vector<float> text){
    numVert[0] = vert.size();
    numVert[1] = normal.size();
    numVert[2] = text.size();
    prepareBuffer(vert, normal, text);
}

Shape :: Shape(string textFile, vector<Point*> vert, vector<Point*> normal, vector<float> text){
    numVert[0] = vert.size();
    numVert[1] = normal.size();
    numVert[2] = text.size();
    prepareBuffer(vert, normal, text);
    loadTexture(textFile);
}

void Shape :: setParseMat(Material* c){
    materials = c;
}


void Shape :: prepareBuffer(vector<Point*> vert, vector<Point*> normal, vector<float> text){
    int i = 0;
    float* vertexs = new float[vert.size() * 3];

    for(vector<Point*>::const_iterator vertex_it = vert.begin(); vertex_it != vert.end(); ++vertex_it){
        vertexs[i++] = (*vertex_it)->getX();
        vertexs[i++] = (*vertex_it)->getY();
        vertexs[i++] = (*vertex_it)->getZ();
    }

    i = 0;
    float* normals = new float[normal.size() * 3];

    for(vector<Point*>::const_iterator vertex_it = normal.begin(); vertex_it != normal.end(); ++vertex_it){
        normals[i++] = (*vertex_it)->getX();
        normals[i++] = (*vertex_it)->getY();
        normals[i++] = (*vertex_it)->getZ();
    }

    glGenBuffers(3, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVert[0] * 3, vertexs, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVert[1] * 3, normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVert[2], &(text[0]), GL_STATIC_DRAW);

    free(vertexs);
    free(normals);
}

void Shape :: loadTexture(string textFile){
    unsigned int t, tw, th;
    unsigned char *textData;

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) textFile.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    textData = ilGetData();

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Shape :: draw(){

    materials -> draw();

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if(numVert[1] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if(numVert[2] > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    glDrawArrays(GL_TRIANGLES, 0, (numVert[0]) * 3);
    glBindTexture(GL_TEXTURE_2D, 0);
}