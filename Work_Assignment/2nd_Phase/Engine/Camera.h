#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

class Camera {
    private:
        float radius, speed, alpha, teta, posX, posY, posZ, lookX, lookY, lookZ, mousePosX, mousePosY;
        bool mousePressed;

    public:
        Camera();
        float getPosX();
        float getPosY();
        float getPosZ();
        float getLookX();
        float getLookY();
        float getLookZ();
        void posIniCamera();
        void changeLook(float x, float y, float z);
        void specialKeysCamera(int key);
        void pressMouse(int button, int state, int x, int y);
        void motionMouse(int x, int y);
};

#endif