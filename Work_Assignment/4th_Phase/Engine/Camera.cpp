#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "Camera.h"

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

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

float Camera :: getOrX() {
    return posX + lookX;
}

float Camera :: getOrY() {
    return posY + lookY;
}

float Camera :: getOrZ() {
    return posZ + lookZ;
}

void Camera :: posIniCamera() {
    alpha = 0.0f;
    teta = -M_PI / 6.0f;

    posX = -100.0f;
    posY = 60.0f;
    posZ = 0.0f;

    lookX = float(cos(teta)*cos(alpha));
    lookY = float(sin(teta));
    lookZ = float(cos(teta)*sin(alpha));

    mousePressed = false;
}

void cross(float* a, float* b, float* res){
    res[0] = a[1] * b[2] -a[2] * b[1];
    res[1] = a[2] * b[0] -a[0] * b[2];
    res[2] = a[0] * b[1] -a[1] * b[0];
}

void Camera::specialKeysCamera(int key){

    switch (key){
        case GLUT_KEY_UP: {
            posX += lookX * 1.7f;
            posY += lookY * 1.7f;
            posZ += lookZ * 1.7f;
            break;
        }

        case GLUT_KEY_DOWN: {
            posX -= lookX * 1.7f;
            posY -= lookY * 1.7f;
            posZ -= lookZ * 1.7f;
            break;
        }

        case GLUT_KEY_LEFT: {
            float up[3], dir[3], res[3];
            up[0] = up[2] = 0;
            up[1] = 1;
            dir[0] = lookX;
            dir[1] = lookY;
            dir[2] = lookZ;

            cross(dir, up, res);

            posX -= res[0] * 1.7f;
            posY -= res[1] * 1.7f;
            posZ -= res[2] * 1.7f;
            break;
        }

        case GLUT_KEY_RIGHT: {
            float up[3], dir[3], res[3];
            up[0] = up[2] = 0;
            up[1] = 1;
            dir[0] = lookX;
            dir[1] = lookY;
            dir[2] = lookZ;

            cross(dir,up,res);

            posX += res[0] * 1.7f;
            posY += res[1] * 1.7f;
            posZ += res[2] * 1.7f;
            break;
        }

        case GLUT_KEY_F1: {
            posIniCamera();
            break;
        }

        default:
            break;
    }
}

void Camera :: pressMouse(int button, int state, int x, int y) {

    if(button == GLUT_LEFT_BUTTON){

        if(state == GLUT_UP){
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
            alpha += ((float)x - mousePosX) * 0.001f;
            teta += ((float)y - mousePosY) * 0.001f;
            mousePressed = false;
        }
        else if(state == GLUT_DOWN){
            glutSetCursor(GLUT_CURSOR_NONE);
            mousePosX = (float)x;
            mousePosY = (float)y;
            mousePressed = true;
        }
    }

    else if(button == GLUT_WHEEL_DOWN && state == GLUT_DOWN)
        posY -= 0.5 * 1.7f;

    else if(button == GLUT_WHEEL_UP && state == GLUT_DOWN)
        posY += 0.5 * 1.7f;
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

        lookX = cos(alphaNew) * cos(tetaNew);
        lookY = sin(tetaNew);
        lookZ = sin(alphaNew) * cos(tetaNew);
    }
}