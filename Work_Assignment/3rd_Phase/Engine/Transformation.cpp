#include "Transformation.h"

Transformation :: Transformation(){
}

Transformation :: Transformation(string t, float a, float xx, float yy, float zz) {
    type = t;
    angle = a;
    x = xx;
    y = yy;
    z = zz;
}

Transformation ::Transformation(float ti, vector<Point *> p, bool d, string t) {
    time = ti;
    controlPoints = p;
    setCatmullPoints();
    deriv = d;
    type = t;
}

string Transformation :: getType() {
    return type;
}

float Transformation :: getAngle() {
    return angle;
}

float Transformation :: getX() {
    return x;
}

float Transformation :: getY() {
    return y;
}

float Transformation :: getZ() {
    return z;
}

float Transformation::getTime(){
    return time;
}

bool Transformation::getDeriv() {
    return deriv;
}

float* Transformation::getVetor(){
    return vetor;
}

vector<Point*> Transformation::getPointsCurve(){
    return pointsCurve;
}

void Transformation::normalize(float *a) {
    float n = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/n;
    a[1] = a[1]/n;
    a[2] = a[2]/n;
}

void Transformation::cross(float *a, float *b, float *res)
{
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void Transformation::rotMatrix(float *r, float *xx, float *yy, float *zz){
    r[0] = xx[0]; r[1] = xx[1]; r[2] = xx[2]; r[3] = 0;
    r[4] = yy[0]; r[5] = yy[1]; r[6] = yy[2]; r[7] = 0;
    r[8] = zz[0]; r[9] = zz[1]; r[10] = zz[2]; r[11] = 0;
    r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;
}

void Transformation :: multMatrixVector(float *m, float *v, float *res) {
    for(int i = 0; i < 4; ++i){
        res[i] = 0;
        for(int j = 0; j < 4; ++j)
            res[i] += v[j] * m[i * 4 + j];
    }
}

void Transformation :: getCatmullRomPoint(float t, int *indexes, float *p, float *der) {
    float m[4][4] = {{ -0.5f, 1.5f, -1.5f, 0.5f },
                     { 1.0f, -2.5f, 2.0f, -0.5f },
                     { -0.5f, 0.0f, 0.5f, 0.0f },
                     { 0.0f, 1.0f, 0.0f, 0.0f }
    };

    float px[4],py[4],pz[4];
    for(int i = 0; i < 4 ; i++){
        px[i] = controlPoints[indexes[i]]->getX();
        py[i] = controlPoints[indexes[i]]->getY();
        pz[i] = controlPoints[indexes[i]]->getZ();
    }

    float a[4][4];
    multMatrixVector(*m, px, a[0]);
    multMatrixVector(*m, py, a[1]);
    multMatrixVector(*m, pz, a[2]);

    float T[4] = { t*t*t, t*t, t, 1};
    multMatrixVector(*a, T, p);

    float Tdev[4] = { 3*T[1] , 2*T[2], 1 , 0};
    multMatrixVector(*a, Tdev, der);
}

void Transformation::getGlobalCatmullRomPoint(float gt, float *p, float *der) {
    int num = controlPoints.size();
    float t = gt * (float)num;
    int index = floor(t);

    t = t - (float)index;

    int indexes[4];
    indexes[0] = (index + num - 1)  % num;
    indexes[1] = (indexes[0]+1)     % num;
    indexes[2] = (indexes[1]+1)     % num;
    indexes[3] = (indexes[2]+1)     % num;

    getCatmullRomPoint(t, indexes, p, der);
}

void Transformation::setCatmullPoints(){
    float ponto[4];
    float der[4];

    for(float i = 0; i < 1; i+=0.01){
        getGlobalCatmullRomPoint(i, ponto, der);
        Point *p = new Point(ponto[0], ponto[1], ponto[2]);
        pointsCurve.push_back(p);
    }
}