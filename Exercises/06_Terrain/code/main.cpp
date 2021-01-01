#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#endif

float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

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

//Devolve altura(pixelValue) do terreno em determinada posição da matriz XZ
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


void init() {

//  Load the height map "terreno.jpg"
    carregarImagem();
//  Build the vertex arrays
    Build_VBOterreno(tw,th);

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_LINE);
}

void drawTerrain() {
    // colocar aqui o código de desenho do terreno usando VBOs com TRIANGLE_STRIPS

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    // Desenho dos triângulos presentes no VBO. O segundo parâmetro indica o vértice inicial, e o último o número de vértices a desenhar.
    // Como estamos a desenhar no modo GL_TRIANGLE_STRIP temos de desenhar uma 'tira' de cada vez
    for (int i=0; i<th; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i*tw*2, 2*tw);
    }

}


void renderScene(void) {

    float pos[4] = {-1.0, 1.0, 1.0, 0.0};

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    ////desenho do terreno
    drawTerrain();

    // just so that it renders something before the terrain is built
    // to erase when the terrain is ready
    glColor3f(0.0f, 0.0f, 1.0f);
    glutWireTeapot(2.0);

// End of frame
    glutSwapBuffers();
}

void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}

void processMouseButtons(int button, int state, int xx, int yy) {

    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}

void processMouseMotion(int xx, int yy) {

    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux *                                sin(betaAux * 3.14 / 180.0);
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
    glutKeyboardFunc(processKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

    //  Devil - get access to IL- main DevIL library functions(https://moddb.fandom.com/wiki/DevIL:Tutorials:Basics)
    ilInit();

    glewInit();
    init();

// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}

