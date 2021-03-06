#include "Patch.h"

Patch :: Patch(){
}

Patch :: Patch(vector<Point> p){
    controlPoints = p;
}

void Patch :: multMatrixVector(float *m, float *v, float *res){
    for(int i = 0; i < 4; ++i){
        res[i] = 0;
        for(int j = 0; j < 4; ++j)
            res[i] += v[j] * m[i * 4 + j];
    }
}

Patch ::Patch(int tess, string file){
    tesselation = tess;
    parsePatchFile(file);
}

void Patch :: parsePatchFile(string filename){
    int i;
    string line, x, y, z;
    string fileDir = "../../Files3d/" + filename;
    ifstream file(fileDir);

    if(file.is_open()){
        getline(file, line);
        nPatches = stoi(line);

        for(i = 0; i < nPatches; i++){
            vector<int> patchIndex;

            if(getline(file, line)){
                char* str = strdup(line.c_str());
                char* token = strtok(str, " ,");

                while(token != NULL){
                    patchIndex.push_back(atoi(token));
                    token = strtok(NULL, " ,");
                }

                patches[i] = patchIndex;
                free(str);
            }
            else
                cout << "Incapaz de obter todos os patchIndex." << endl;
        }

        getline(file, line);
        nPoints = stoi(line);

        for(i = 0; i < nPoints; i++){
            if(getline(file, line)){
                char* str = strdup(line.c_str());
                char* token = strtok(str, " ,");
                float xx = atof(token);
                token = strtok(NULL, " ,");
                float yy = atof(token);
                token = strtok(NULL, " ,");
                float zz = atof(token);
                Point *p = new Point(xx,yy,zz);
                controlPoints.push_back(*p);
                free(str);
            }
            else
                cout << "Incapaz de obter todos os controlPoint." << endl;
        }

        file.close();
    }
    else
        cout << "Erro na abertura do ficheiro " << filename << endl;
}

Point* Patch :: getPoint(float ta, float tb, float (*coordX)[4], float (*coordY)[4], float (*coordZ)[4]){
    float x = 0.0f, y = 0.0f, z = 0.0f;

    float a[4] = { ta*ta*ta, ta*ta, ta, 1.0f};
    float b[4] = { tb*tb*tb, tb*tb, tb, 1.0f};

    float am[4];
    multMatrixVector(*m,a,am);

    float bm[4];
    multMatrixVector(*m,b,bm);

    float amCoordenadaX[4], amCoordenadaY[4], amCoordenadaZ[4];
    multMatrixVector(*coordX,am,amCoordenadaX);
    multMatrixVector(*coordY,am,amCoordenadaY);
    multMatrixVector(*coordZ,am,amCoordenadaZ);

    for (int i = 0; i < 4; i++){
        x += amCoordenadaX[i] * bm[i];
        y += amCoordenadaY[i] * bm[i];
        z += amCoordenadaZ[i] * bm[i];
    }

    Point *p = new Point(x,y,z);
    return p;
}

float* Patch :: getTangent(float tu, float tv, float mX[4][4], float mY[4][4], float mZ[4][4], int type){
    float u[4], v[4];

    if(type == 0) {
        u[0] = 3.0f * tu * tu;
        u[1] = 2.0f * tu;
        u[2] = 1.0f;
        u[3] = 0.0f;

        v[0] = tv * tv * tv;
        v[1] = tv * tv;
        v[2] = tv;
        v[3] = 1.0f;
    }
    else {
        u[0] = tu * tu * tu;
        u[1] = tu * tu;
        u[2] = tu;
        u[3] = 1.0f;

        v[0] = 3.0f * tv * tv;
        v[1] = 2.0f * tv;
        v[2] = 1.0f;
        v[3] = 0.0f;
    }

    float uM[4];
    multMatrixVector(*m,u,uM);

    float Mv[4];
    multMatrixVector(*m,v,Mv);

    float matX[4], matY[4], matZ[4];
    multMatrixVector(*mX,uM,matX);
    multMatrixVector(*mY,uM,matY);
    multMatrixVector(*mZ,uM,matZ);

    float *tang = (float *) calloc(3, sizeof(float));
    for (int i = 0; i < 4; i++){
        tang[0] += matX[i] * Mv[i];
        tang[1] += matY[i] * Mv[i];
        tang[2] += matZ[i] * Mv[i];
    }
    return tang;
}

void Patch::normalize(float *a){
    float n = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/n;
    a[1] = a[1]/n;
    a[2] = a[2]/n;
}

void Patch::cross(float *a, float *b, float *res){
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void Patch :: getPatchPoints(int patch, vector<Point>* points, vector<float>* texture, vector<Point>* normal){
    vector<int> indexesControlPoints = patches.at(patch);

    float coordenadasX[4][4], coordenadasY[4][4], coordenadasZ[4][4];
    float u,v,uu,vv;
    float t = 1.0f /(float)tesselation;
    int pos = 0;
    float *tangenteU,*tangenteV,res[3];
    Point *p0,*p1,*p2,*p3;
    Point *n0,*n1,*n2,*n3;

    for (int i = 0; i < 4; i++){

        for (int j = 0; j < 4; j++){
            Point controlPoint = controlPoints[indexesControlPoints[pos]];
            coordenadasX[i][j] = controlPoint.getX();
            coordenadasY[i][j] = controlPoint.getY();
            coordenadasZ[i][j] = controlPoint.getZ();
            pos++;
        }
    }

    for(int i = 0; i < tesselation; i++){

        for (int j = 0; j < tesselation; j++){
            u = (float)i*t;
            v = (float)j*t;
            uu = (float)(i+1)*t;
            vv = (float)(j+1)*t;

            p0 = getPoint(u, v, coordenadasX, coordenadasY, coordenadasZ);
            tangenteU = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 0);
            tangenteV = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n0 = new Point(res[0],res[1],res[2]);

            p1 = getPoint(u, vv, coordenadasX, coordenadasY, coordenadasZ);
            tangenteU = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 0);
            tangenteV = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n1 = new Point(res[0],res[1],res[2]);

            p2 = getPoint(uu, v, coordenadasX, coordenadasY, coordenadasZ);
            tangenteU = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 0);
            tangenteV = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n2 = new Point(res[0],res[1],res[2]);

            p3 = getPoint(uu, vv, coordenadasX, coordenadasY, coordenadasZ);
            tangenteU = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 0);
            tangenteV = getTangent(u, v, coordenadasX, coordenadasY, coordenadasZ, 1);
            cross(tangenteU, tangenteV, res);
            normalize(res);
            n3 = new Point(res[0],res[1],res[2]);


            points->push_back(*p0); points->push_back(*p2); points->push_back(*p1);
            points->push_back(*p1); points->push_back(*p2); points->push_back(*p3);

            normal->push_back(*n0); normal->push_back(*n2); normal->push_back(*n1);
            normal->push_back(*n1); normal->push_back(*n2); normal->push_back(*n3);

            texture->push_back(1-u); texture->push_back(1-v);
            texture->push_back(1-uu); texture->push_back(1-v);
            texture->push_back(1-u); texture->push_back(1-vv);
            texture->push_back(1-u); texture->push_back(1-vv);
            texture->push_back(1-uu); texture->push_back(1-v);
            texture->push_back(1-uu); texture->push_back(1-vv);
        }
    }
}

void Patch :: BezierModelGenerator(vector<Point> *vert, vector<Point> *normal, vector<float> *texture){

    for(int i = 0; i < nPatches; i++)
        getPatchPoints(i, vert, texture, normal);
}