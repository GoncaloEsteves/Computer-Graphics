#include "Patch.h"

Patch :: Patch(){
}

Patch :: Patch(vector<Point> p){
    controlPoints = p;
}

void Patch :: multMatrixVector(float *m, float *v, float *res) {
    for(int i = 0; i < 4; ++i){
        res[i] = 0;
        for(int j = 0; j < 4; ++j)
            res[i] += v[j] * m[i * 4 + j];
    }
}

Patch ::Patch(int tess, string file) {
    tesselation = tess;
    parsePatchFile(file);
}

void Patch :: parsePatchFile(string filename) {
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

Point* Patch :: getPoint(float ta, float tb, float (*coordX)[4], float (*coordY)[4], float (*coordZ)[4]) {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float m[4][4] = {{-1.0f,  3.0f, -3.0f,  1.0f},
                     { 3.0f, -6.0f,  3.0f,  0.0f},
                     {-3.0f,  3.0f,  0.0f,  0.0f},
                     { 1.0f,  0.0f,  0.0f,  0.0f}};

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

    for (int i = 0; i < 4; i++)
    {
        x += amCoordenadaX[i] * bm[i];
        y += amCoordenadaY[i] * bm[i];
        z += amCoordenadaZ[i] * bm[i];
    }

    Point *p = new Point(x,y,z);
    return p;
}

vector<Point> Patch::getPatchPoints(int patch) {
    vector<Point> points;
    vector<int> indexesControlPoints = patches.at(patch);

    float coordenadasX[4][4], coordenadasY[4][4], coordenadasZ[4][4];
    float u,v,uu,vv;
    float t = 1.0f /(float)tesselation;
    int pos = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Point controlPoint = controlPoints[indexesControlPoints[pos]];
            coordenadasX[i][j] = controlPoint.getX();
            coordenadasY[i][j] = controlPoint.getY();
            coordenadasZ[i][j] = controlPoint.getZ();
            pos++;
        }
    }

    for(int i = 0; i < tesselation; i++)
    {
        for (int j = 0; j < tesselation; j++)
        {
            u = (float)i*t;
            v = (float)j*t;
            uu = (float)(i+1)*t;
            vv = (float)(j+1)*t;
            Point *p0,*p1,*p2,*p3;
            p0 = getPoint(u, v, coordenadasX, coordenadasY, coordenadasZ);
            p1 = getPoint(u, vv, coordenadasX, coordenadasY, coordenadasZ);
            p2 = getPoint(uu, v, coordenadasX, coordenadasY, coordenadasZ);
            p3 = getPoint(uu, vv, coordenadasX, coordenadasY, coordenadasZ);

            points.push_back(*p0); points.push_back(*p2); points.push_back(*p1);
            points.push_back(*p1); points.push_back(*p2); points.push_back(*p3);
        }
    }
    return points;
}

vector<Point> Patch :: BezierModelGenerator() {
    vector<Point> res;

    for(int i = 0; i < nPatches; i++){
        vector<Point> aux = getPatchPoints(i);
        res.insert(res.end(), aux.begin(), aux.end());
    }

    return res;
}