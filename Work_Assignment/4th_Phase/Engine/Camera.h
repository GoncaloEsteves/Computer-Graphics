#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

class Camera {
    private:
        float alpha, teta, posX, posY, posZ, lookX, lookY, lookZ, mousePosX, mousePosY;
        bool mousePressed;

    public:
        Camera();
        float getPosX();
        float getPosY();
        float getPosZ();
        float getOrX();
        float getOrY();
        float getOrZ();
        void posIniCamera();
        void specialKeysCamera(int key);
        void pressMouse(int button, int state, int x, int y);
        void motionMouse(int x, int y);
};

#endif