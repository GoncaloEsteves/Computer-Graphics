#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

//para declarar só "vector" em vez de std::vector...
using std::vector;


// FPS - mostrador ****** variáveis ************************************
int frame = 0;
int time = 0;
int timebase = 0;
// *********************************************************

// A variável vertices deverá ser global já que será necessária na função renderScene. A variável
// vertices ficará com o valor 1 por ser o primeiro VBO a ser criado. Este valor representa o número do
// “slot” alocado e é usado sempre que nos quisermos referir ao VBO. A variável verticeCount, também
// global, irá guardar o número total de vértices. Iremos precisar de ambas estas variáveis para desenhar o
// triângulo na função renderScene.//
GLuint vertices, verticeCount;

// criar um vector com os dados dos pontos
//vector<float> p;


float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {

    camX = radius * cos(beta) * sin(alfa);
    camY = radius * sin(beta);
    camZ = radius * cos(beta) * cos(alfa);
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

/*-----------------------------------------------------------------------------------
Método normal da função cilindro (sem VBOs) - usando glVertex3f
-----------------------------------------------------------------------------------*/
void cylinder(float radius, float height, int sides) {
    int i;
    float step;

    step = 360.0 / sides;

    glBegin(GL_TRIANGLES);

    // top
    for (i = 0; i < sides; i++) {
        glVertex3f(0, height*0.5, 0);
        glVertex3f(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
        glVertex3f(cos((i+1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
    }

    // bottom
    for (i = 0; i < sides; i++) {
        glVertex3f(0, -height*0.5, 0);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
        glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
    }

    // body
    for (i = 0; i <= sides; i++) {

        glVertex3f(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
        glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);

        glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
        glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
    }
    glEnd();
}

/*-----------------------------------------------------------------------------------
    Draw Cylinder with strips and fans (Sem VBOs)
-----------------------------------------------------------------------------------*/
// explicações basícas 1 tipos de triângulos : https://tinyurl.com/rlgnb5n
// explicações basícas 2: https://tinyurl.com/wymm8sd
void cylinder0(float radius, float height, int sides) {

    int i;
    float step;

    step = 360.0/sides;

    glColor3f(1,0,0);
    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(0,height*0.5,0);
    for (i=0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
    }
    glEnd();

    glColor3f(0,1,0);
    glBegin(GL_TRIANGLE_FAN); // A primitiva GL_TRIANGLE_FAN é usada para se conectar vários triângulos em torno de um ponto central, tal como um leque (“fan”):

    glVertex3f(0,-height*0.5,0);
    for (i=0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
    }
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_TRIANGLE_STRIP); //A primitiva GL_TRIANGLE_STRIP é usada para se desenhar uma tira de triângulos (triângulos conectados sequencialmente).

    for (i=0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
        glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
    }
    glEnd();
}


/*-----------------------------------------------------------------------------------
    Cilindro com VBOs - sem indices
-----------------------------------------------------------------------------------*/
void prepareDataCylinder(float radius, float height, int sides) {

    //// 1) criar um vector com para as coordenadas dos pontos --------------
    vector<float> p;

    int i;
    float step;

    step = 360.0 / sides;

    //preenchimento do vector com os pontos do cilindro
    // top
    for (i = 0; i < sides; i++) {
        p.push_back(0);
        p.push_back(height*0.5);
        p.push_back(0);

        p.push_back(cos(i * step * M_PI / 180.0)*radius);
        p.push_back(height*0.5);
        p.push_back(-sin(i * step *M_PI / 180.0)*radius);

        p.push_back(cos((i+1) * step * M_PI / 180.0)*radius);
        p.push_back(height*0.5);
        p.push_back(-sin((i + 1) * step *M_PI / 180.0)*radius);
    }

    // bottom
    for (i = 0; i < sides; i++) {
        p.push_back(0);
        p.push_back(-height*0.5);
        p.push_back(0);

        p.push_back(cos((i + 1) * step * M_PI / 180.0)*radius);
        p.push_back(-height*0.5);
        p.push_back(-sin((i + 1) * step *M_PI / 180.0)*radius);

        p.push_back(cos(i * step * M_PI / 180.0)*radius);
        p.push_back(-height*0.5);
        p.push_back(-sin(i * step *M_PI / 180.0)*radius);
    }

    // body
    for (i = 0; i <= sides; i++) {
        p.push_back(cos(i * step * M_PI / 180.0)*radius); p.push_back(height*0.5); p.push_back(-sin(i * step *M_PI / 180.0)*radius);
        p.push_back(cos(i * step * M_PI / 180.0)*radius); p.push_back(-height*0.5); p.push_back(-sin(i * step *M_PI / 180.0)*radius);
        p.push_back(cos((i + 1) * step * M_PI / 180.0)*radius); p.push_back(height*0.5); p.push_back(-sin((i + 1) * step *M_PI / 180.0)*radius);

        p.push_back(cos(i * step * M_PI / 180.0)*radius); p.push_back(-height*0.5); p.push_back(-sin(i * step *M_PI / 180.0)*radius);
        p.push_back(cos((i + 1) * step * M_PI / 180.0)*radius); p.push_back(-height*0.5); p.push_back(-sin((i + 1) * step *M_PI / 180.0)*radius);
        p.push_back(cos((i + 1) * step * M_PI / 180.0)*radius); p.push_back(height*0.5); p.push_back(-sin((i + 1) * step *M_PI / 180.0)*radius);
    }

    // atualização do número de vértices/Pontos (vertice =  3 coordenadas x,y,z)
    verticeCount = p.size() / 3;

    //// 2) criar o VBO -----------------------------------------------------------------------------

    glGenBuffers(1, &vertices); // Enable buffer vertices

    //// 3) copiar o vector para a memória gráfica --------------------------------------------------
    // copiar o vector para a memória gráfica
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(
            GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
            sizeof(float) * p.size(), // tamanho do vector em bytes
            p.data(), // os dados do array associado ao vector
            GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
}

///// -----------------------------------------------------------------------------------
////  Draw Cylinder
void drawVBOCylinder() {
    //  Bind and semantics
    glBindBuffer(GL_ARRAY_BUFFER, vertices); //No excerto de código acima primeiro indicamos que vertices é o VBO activo. Aqui sim, o tipo do buffer é relevante. Para arrays com propriedades dos vértices, como as posições, o tipo deve ser GL_ARRAY_BUFFER.
    glVertexPointer(3, GL_FLOAT, 0, 0); // Define a semântica dos dados no VBO. Ao copiarmos os dados, com a função glBufferData, não passámos nenhuma informação sobre a estrutura dos dados, neste caso que um vértice contem 3 floats. Poderiam ser 4 doubles, ou outra variante qualquer. A função glBufferData só copia bytes sem associar nenhuma semântica a esses bytes. A função glVertexPointer é usada para fornecer essa semântica (os dois últimos parâmetros não são relevantes para este exemplo). Neste caso estamos a dizer que um vértice é constituído por 3 floats.

    //  Draw
    glDrawArrays(GL_TRIANGLES, 0, verticeCount); // Desenho dos triângulos presentes no VBO. O segundo parâmetro indica o vértice inicial, e o último o número de vértices a desenhar.
}
//// -----------------------------------------------------------------------------------


void renderScene(void) {

    float pos[4] = { 1.0, 1.0, 1.0, 0.0 };

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);

    //// ---> Testes ////////////////////////////////////////////////////////////////////////
    //// cylinder sem vbos ------------------------------------------------------------------
    //cylinder(1, 2, 20000);
    //// cylinder sem vbos with strips and fans ---------------------------------------------
    //cylinder0(1, 2, 20000);
    //// cylinder com vbos ------------------------------------------------------------------
    drawVBOCylinder();
    /////////////////////////////////////////////////////////////////////////////////////////


    // FPS ****** func ************************************
    float fps;
    char str[100];

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame*1000.0 / (time - timebase);
        timebase = time;
        frame = 0;
        sprintf(str, "%f fps", fps);
        glutSetWindowTitle(str);
    }
    // *********************************************************
    // End of frame
    glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {

        case GLUT_KEY_RIGHT:
            alfa -= 0.1; break;
            alfa -= 0.1; break;

        case GLUT_KEY_LEFT:
            alfa += 0.1; break;

        case GLUT_KEY_UP:
            beta += 0.1f;
            if (beta > 1.5f)
                beta = 1.5f;
            break;

        case GLUT_KEY_DOWN:
            beta -= 0.1f;
            if (beta < -1.5f)
                beta = -1.5f;
            break;

        case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
            if (radius < 0.1f)
                radius = 0.1f;
            break;

        case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
    }
    spherical2Cartesian();
    glutPostRedisplay();

}


void printInfo() {

    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));

    printf("\nUse Arrows to move the camera up/down and left/right\n");
    printf("PageUp and PageDown control the distance from the camera to the origin");

    printf ("\n vertices %d\n",verticeCount); //----------------------------------------------------------------------------add
}


int main(int argc, char **argv) {

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene); //add -----------------------
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

#ifndef __APPLE__
    glewInit();
#endif

//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glPolygonMode(GL_FRONT, GL_LINE);

    spherical2Cartesian();

    // Do once during initialization of the app
    glEnableClientState(GL_VERTEX_ARRAY); 

    ////Construir VBO || Esta função só é necessária chamar 1 vez
    // preparação dos dados com vbos para cilindro , a função de desenho drawVBOCylinder(), na renderscene, irá buscar estes dados alocados.
    prepareDataCylinder(1, 2, 20000);


    printInfo();

// enter GLUT's main cycle
    glutMainLoop();

// FPS ****** time *****************************************
    time = glutGet(GLUT_ELAPSED_TIME);
// *********************************************************

    return 1;
}
