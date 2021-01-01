#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <string>
#include <time.h>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#endif

#define speedKeys 0.8
//// T e c l a d o  ------------------------------------
// semaforo press/nopress das teclas
bool keys[256];
bool specialKeys[256];

GLdouble px = 0.0f, py = 0, pz = 0.0f,
        dx = 0, dy = 0, dz = -1,
        alfa = M_PI, beta = 0.0f;

//// F i g u r a s ------------------------------------
float * trees;

int nTrees = 200;
int nTeapot = 10;
int radiusTeaInt = 35;
int radiusTeaExt = 65;
float radiusBig = 50;
float angleTeaInt = 0;
float angleTeaExt = 0;
////******************************************************************

////Variaveis para VBOs ---------------------------------------- - - - -
/* (GLuint: 32bits-Unsigned binary integer == GL_UNSIGNED_INT)
   A variável buffers ficará com o valor 1 por ser o primeiro VBO a ser criado. Este valor representa o número do
   “slot” alocado e é usado sempre que nos quisermos referir ao VBO.*/
GLuint n_coords, buffers[1];

/////Variaveis para carregar imagem----------------------------- - - - -
unsigned int t, tw, th;    //t-ref img; tw-ref imgWidth; th-ref imgHeight
unsigned char *imageData;  // Array linear onde vão ficar o valores dos pixeis da nossa imagem

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective
    gluPerspective(45,ratio,1,1000);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void carregarImagem(){
    //The Num parameter specifies how many images you will load with DevIL.The Images parameter is set to the image names.
    ilGenImages(1,&t);
    //Vincular imagem 't' na devil internal image stack, para executar todas as operacoes subsequentes nessa imagem.
    ilBindImage(t);
    // terreno.jpg is the image containing our height map (leitura da imagem)
    ilLoadImage((ILstring)"terreno.jpg");
    // convert the image to single channel per pixel, with values ranging between 0 and 255
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    /*important: check tw and th values
    // both should be equal to 256
    // if not there was an error loading the image
    // most likely the image could not be found*/
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    // imageData is a LINEAR array with the pixel values
    imageData = ilGetData();
}

//Devolve altura(pixelValue) do terreno em determinada posição da matriz XZ -|-
float h(int x, int z){                                //x1  x2
    //Numeros do imageData vão de 0 a 255 ||         z0|210|...|
    // -> 0-255 scale:
    //return (imageData[x + z*tw]);
    //Note: scale the heights from 0-255 (pixel intensity) to 0 - 30 meters for a more appropriate rendering
    // -> 0-30 scale:
    //return ((imageData[x + z*tw]) * 30 / 255);
    // -> 0-120 scale:
    return ((imageData[x + z*tw]) * 120 / 255);
}

//Devolve altura dentro das células da matriz |_|
float hf(float x, float z) {
    float meiaLarguraX = tw / 2.0f;
    float meiaAlturaZ = th / 2.0f;
    x += meiaLarguraX;
    z += meiaAlturaZ;

    // vertices do quadrado celula da matriz:
    // metodo math.floor : Retorna o maior valor integral menor ou igual ao numero especificado.
    int x1 = floor(x);
    int z1 = floor(z);
    int x2 = x1 + 1;
    int z2 = z1 + 1;

    // parte fracionaria do ponto x e z
    float fx = x - x1;
    float fz = z - z1;

    // altura de (x1,z) usando interpolacao linear entre as alturas (x1,z1) e (x1,z2).
    float height_x1_z = h(x1,z1) * (1-fz) + h(x1,z2) * fz;
    // altura de (x2,z) usando interpolacao linear entre as alturas (x2,z1) e (x2,z2).
    float height_x2_z = h(x2,z1) * (1-fz) + h(x2,z2) * fz;

    // calculo da altura no ponto (x,z) usando interpolacao linear entre as alturas de (x1,z) e (x2,z)
    float height_xz = height_x1_z * (1-fx) + height_x2_z * fx;

    return height_xz;
}

void Build_VBOterreno(float largura,float altura){

    float meiaLarguraX = (float)tw/2;
    float meiaAlturaZ = (float)th/2;

    // 1) criar um array para as coordenadas dos pontos -------------- - - -
    float *vertexB = NULL;
    int size = 0;

    //numero de coordenadas no array
    n_coords = (tw - 1)*th * 6;

    vertexB = (float*)malloc(sizeof(float) * n_coords);

    //Activar Buffers
    glEnableClientState(GL_VERTEX_ARRAY);


    //preenchimento do array com os pontos do terreno
    for (int x = 0; x < tw-1; x++) {
        for (int z = 0; z < th; z++) {
            vertexB[size++] = ((x+1) - meiaLarguraX);
            vertexB[size++] = h(x + 1, z);
            vertexB[size++] = (z - meiaAlturaZ);

            vertexB[size++] = (x - meiaLarguraX);
            vertexB[size++] = h(x , z);
            vertexB[size++] = (z - meiaAlturaZ);
        }
    }

    // 2) criação do VBO ------------------------------------------------ - - -
    glGenBuffers(1, buffers); // Enable buffer "buffers" com 1 VBO neste caso

    // 3) copiar o array para a memória gráfica ------------------------- - - -
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertexB, GL_STATIC_DRAW);
}

void buildTrees(int n){
    trees = (float*) malloc(n*2*sizeof(float));

    float x, z;
    int i, l = 0;
    srand((unsigned int) time(NULL));

    for(i = 0; i < n; i++){
        x = 0;
        z = 0;

        while(x*x + z*z < radiusBig*radiusBig){
            x = (((float) rand()/(float) (RAND_MAX)) * 200) - 100;
            z = (((float) rand()/(float) (RAND_MAX)) * 200) - 100;
        }

        trees[l++] = x;
        trees[l++] = z;
    }
}


void init() {
//  Resetar press teclas da camara
    for (int i = 0; i < 256; i++)
        specialKeys[i] = keys[i] = false;

//  Load the height map "terreno.jpg"
    carregarImagem();
//  Build the vertex arrays
    Build_VBOterreno(tw,th);

//  Build (x,z) random coordenates trees
    buildTrees(nTrees);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //wglPolygonMode(GL_FRONT, GL_LINE);
}

void drawTerrain() {
    glColor3f(0.2f, 0.8f, 0.2f); //verde
    // colocar aqui o código de desenho do terreno usando VBOs com TRIANGLE_STRIPS

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // Desenho dos triângulos presentes no VBO. O segundo parâmetro indica o vértice inicial, e o último o número de vértices a desenhar.
    // Como estamos a desenhar no modo GL_TRIANGLE_STRIP temos de desenhar uma 'tira' de cada vez
    for (int i=0; i<th; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*tw*2, 2*tw);
    }
}


void drawTrees(int n){
    int i, l = 0;
    float x, z;

    for(i = 0; i < n; i++){
        x = trees[l++];
        z = trees[l++];

        glPushMatrix();
        // com a devida altura do terreno  hf(x,z)
        glTranslatef(x, hf(x, z), z);
        glRotatef(-90, 1, 0, 0);

        glColor3f(0.48627,0.21176,0.1490);
        glutSolidCone(1, 3, 8, 4);

        glTranslatef(0,0,2.5);
        glColor3f(0.26666,0.4,0.1333);
        glutSolidCone(3, 5, 8, 4);

        glPopMatrix();
    }
}

void drawTorus(){
    glPushMatrix();
    glTranslatef(0, hf(0, 0), 0);
    glColor3f(1, 0, 0.5);
    glutSolidTorus(2,3,20,20);
    glPopMatrix();
}


void drawTeapotsInt(int n){
    int i = 0;
    float x,z;


    for (i=0;  i< n+1; i++) {

        x = (radiusTeaInt) * sin(angleTeaInt + (((M_PI * 2)/n) * i));
        z = (radiusTeaInt) * cos(angleTeaInt + (((M_PI * 2)/n) * i));

        glPushMatrix();
        glTranslatef(x, hf(x,z)+3, z);
        glRotatef(-360.0f/i + 90, 0, 2, 0);
        glColor3f(0, 0, 1);\
        glutSolidTeapot(3);
        glPopMatrix();

        angleTeaInt += 0.0005f;
    }
}

void drawTeapotsExt(int n){
    int i = 0;
    float x,z;


    for (i=0;  i< n+1; i++) {

        x = (radiusTeaExt) * sin(angleTeaExt + (((M_PI * 2)/n) * i));
        z = (radiusTeaExt) * cos(angleTeaExt + (((M_PI * 2)/n) * i));

        glPushMatrix();
        glTranslatef(x, hf(x,z)+3, z);
        glRotatef(-360.0f/i + 90, 0, 2, 0);
        glColor3f(1, 0, 0);
        glutSolidTeapot(3);
        glPopMatrix();

        angleTeaExt += 0.0005f;
    }
}


void renderScene(void) {

    float pos[4] = {-1.0, 1.0, 1.0, 0.0};

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

/*    gluLookAt(camX, camY, camZ,
              px + dx, py + dy, pz + dz,
              0.0f, 1.0f, 0.0f);
*/
    gluLookAt(px, py, pz,
              px + dx, py + dy, pz + dz,
              0.0f, 1.0f, 0.0f);

    ////desenho do terreno
    drawTerrain();

    ////desenho das Arvores
    drawTrees(nTrees);
    ////desenho do Torus
    drawTorus();
    ////desenho dos TeaPots interiores
    drawTeapotsInt(nTeapot);
    ////desenho dos TeaPots exteriores
    drawTeapotsExt(nTeapot);

// End of frame
    glutSwapBuffers();
}


///// C A M A R A -------------------------------------------------
void updateAngles() {
    dz = 1 * cosf(beta) * cosf(alfa);
    dx = 1 * cosf(beta) * sinf(alfa);
    dy = 1 * sin(beta);
}

// Functions to process keyboard events
void keyReleased(unsigned char key, int x, int y) {
    key = tolower(key);
    keys[key] = false;
}

void keyPressed(unsigned char key, int x, int y) {
    key = tolower(key);
    keys[key] = true;
}

void specialKeyReleased(int key, int x, int y) {
    specialKeys[key] = false;
}

void specialKeyPressed(int key, int x, int y) {
    specialKeys[key] = true;
}

void processDownKeys() {
    // bool de tecla a ser ou não pressionada
    bool reDisplay = false;

    // "tecla space (27) saida de func com sucesso"
    if (keys[27]) {
        exit(EXIT_SUCCESS);
    }

    if (keys['w']) {
        px += dx * speedKeys;
        py += dy * speedKeys;
        pz += dz * speedKeys;

        reDisplay = true;
    }

    if (keys['s']) {
        px -= dx * speedKeys;
        py -= dy * speedKeys;
        pz -= dz * speedKeys;

        reDisplay = true;
    }

    if (keys['a']) {
        px += dz * speedKeys;
        pz += -dx * speedKeys;

        reDisplay = true;
    }
    if (keys['d']) {
        px -= dz * speedKeys;
        pz -= -dx * speedKeys;

        reDisplay = true;
    }

    // 'espaco' = reset cam
    if (keys[' ']) {
        px = 0;
        py = 0;
        pz = 10;
        dx = 0;
        dy = 0;
        dz = -1;
        alfa = M_PI;
        beta = 0.0f;
        reDisplay = true;
    }

    if (specialKeys[GLUT_KEY_UP]) {
        if (beta + speedKeys * 0.1 < M_PI_2) {
            beta += speedKeys * 0.1;
            updateAngles();
            reDisplay = true;
        }
    }

    if (specialKeys[GLUT_KEY_DOWN]) {
        if (beta - speedKeys * 0.1 > -M_PI_2) {
            beta -= speedKeys * 0.1;
            updateAngles();
            reDisplay = true;
        }
    }

    if (specialKeys[GLUT_KEY_RIGHT]) {
        alfa -= speedKeys * 0.1;
        updateAngles();
        reDisplay = true;
    }
    if (specialKeys[GLUT_KEY_LEFT]) {
        alfa += speedKeys * 0.1;
        updateAngles();
        reDisplay = true;
    }

    //// variação da altura da camara com a altura do terreno
    py = 2.5f + hf(px, pz);

    glutPostRedisplay();
}

void timer(int) {
    processDownKeys();
    //glutTimerFunc registers a timer callback to be triggered in a specified number of milliseconds.
    //void glutTimerFunc(unsigned int msecs, void (*func)(int value), value);
    glutTimerFunc(1, timer, 0);
}

int main(int argc, char **argv) {

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("CG@DI-UM");


// Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    //keyboard -----
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);

    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF); //desactivar repeatMode das teclas para todas as telas


    //  Devil - get access to IL- main DevIL library functions(https://moddb.fandom.com/wiki/DevIL:Tutorials:Basics)
    ilInit();

    glewInit();
    init();

    // timer para multiplas teclas pressionadas
    timer(0);

// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}

