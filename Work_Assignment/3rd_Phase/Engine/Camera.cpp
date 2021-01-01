#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "Camera.h"

Camera :: Camera(){
    posIniCamera();
}

float Camera :: getPosX() {
    return posX;
}

float Camera :: getPosY() {
    return posY;
}

float Camera :: getPosZ() {
    return posZ;
}

float Camera :: getLookX() {
    return lookX;
}

float Camera :: getLookY() {
    return lookY;
}

float Camera :: getLookZ() {
    return lookZ;
}

void Camera :: posIniCamera() {
    radius = 120.0f;
    speed = 0.5f;
    alpha = 4.3f;
    teta = 0.3f;

    lookX = 0; lookY = 0; lookZ = 0;

    posX = lookX + radius*sin(alpha)*cos(teta);
    posY = lookX + radius*sin(teta);
    posZ = lookZ + radius*cos(alpha)*cos(teta);

    mousePressed = false;
}

void Camera :: changeLook(float x, float y, float z) {
    posIniCamera();

    lookX = x;
    lookY = y;
    lookZ = z;

    posX += x;
    posY += y;
    posZ += z;
}

void Camera::specialKeysCamera(int key){
    switch (key){
        case GLUT_KEY_UP:
            if (teta < (M_PI / 2 - speed))
                teta += speed;
            break;

        case GLUT_KEY_DOWN:
            if (teta > -(M_PI / 2 - speed))
                teta -= speed;
            break;

        case GLUT_KEY_LEFT:
            alpha -= speed;
            break;

        case GLUT_KEY_RIGHT:
            alpha += speed;
            break;

        case GLUT_KEY_F1:
            radius -= speed * 2;
            break;

        case GLUT_KEY_F2:
            radius += speed * 2;
            break;

        case GLUT_KEY_F6:
            posIniCamera();
            break;

        default:
            break;
    }

    posX = lookX + radius*sin(alpha)*cos(teta);
    posY = lookY + radius*sin(teta);
    posZ = lookZ + radius*cos(alpha)*cos(teta);
}

void Camera :: pressMouse(int button, int state, int x, int y) {

    if(button == GLUT_LEFT_BUTTON){

        if(state == GLUT_UP){
            alpha += ((float)x - mousePosX) * 0.001f;
            teta += ((float)y - mousePosY) * 0.001f;
            mousePressed = false;
        }
        else if(state == GLUT_DOWN){
            mousePosX = (float)x;
            mousePosY = (float)y;
            mousePressed = true;
        }
    }
}

void Camera :: motionMouse(int x, int y) {

    if(mousePressed){
        float alphaNew, tetaNew;

        alphaNew = alpha + ((float)x - mousePosX) * 0.001f;
        tetaNew = teta + ((float)y - mousePosY) * 0.001f;

        if(tetaNew > M_PI/2 - 0.05f)
            tetaNew = M_PI/2 - 0.05f;
        else if(tetaNew < - M_PI/2 + 0.05f)
            tetaNew = - M_PI/2 + 0.05f;

        posX = lookX + radius * sin(alphaNew) * cos(tetaNew);
        posY = lookY + radius * sin(tetaNew);
        posZ = lookZ + radius * cos(alphaNew) * cos(tetaNew);
    }
}