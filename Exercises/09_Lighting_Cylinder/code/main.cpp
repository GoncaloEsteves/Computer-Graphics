#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <math.h>

#define _PI_ 3.14159

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint vertexCount, buffers[2];

int timebase = 0, frame = 0;


void sphericalToCartesian() {

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



void prepareCilinder(float height, float radius, int sides) {

    float *v;              //array dos vertices
    float *normalB;        //array das normais

    v = (float *)malloc(sizeof(float) * 4 * 3 * 3 * sides);
    normalB = (float *)malloc(sizeof(float) * 4 * 3 * 3 * sides);

    int vertex = 0;
    float delta = 2.0f * _PI_ / sides;

    for (int i = 0; i < sides; ++i) {
        // top
        // central point
        v[vertex*3 + 0] = 0.0f;
        v[vertex*3 + 1] = height /2.0f;
        v[vertex*3 + 2] = 0.0f;

        normalB[vertex * 3 + 0] = 0.0f;
        normalB[vertex * 3 + 1] = 1.0f;
        normalB[vertex * 3 + 2] = 0.0f;

        vertex++;
        v[vertex*3 + 0] = radius * sin( i * delta);
        v[vertex*3 + 1] = height /2.0f;
        v[vertex*3 + 2] = radius * cos( i * delta);

        normalB[vertex * 3 + 0] = 0.0f;
        normalB[vertex * 3 + 1] = 1.0f;
        normalB[vertex * 3 + 2] = 0.0f;

        vertex++;
        v[vertex*3 + 0] = radius * sin( (i+1) * delta);
        v[vertex*3 + 1] = height /2.0f;
        v[vertex*3 + 2] = radius * cos( (i+1) * delta);

        normalB[vertex * 3 + 0] = 0.0f;
        normalB[vertex * 3 + 1] = 1.0f;
        normalB[vertex * 3 + 2] = 0.0f;

        // body
        // tri�ngulo 1
        vertex++;
        v[vertex*3 + 0] = radius * sin( (i+1) * delta);
        v[vertex*3 + 1] = height /2.0f;
        v[vertex*3 + 2] = radius * cos( (i+1) * delta);

        normalB[vertex*3 + 0] = sin( (i+1) * delta);
        normalB[vertex*3 + 1] = 0;
        normalB[vertex*3 + 2] = cos( (i+1) * delta);

        vertex++;
        v[vertex*3 + 0] = radius * sin( i * delta);
        v[vertex*3 + 1] = height /2.0f;
        v[vertex*3 + 2] = radius * cos( i * delta);

        normalB[vertex*3 + 0] = sin(i * delta);
        normalB[vertex*3 + 1] = 0;
        normalB[vertex*3 + 2] = cos(i * delta);

        vertex++;
        v[vertex*3 + 0] = radius * sin( i * delta);
        v[vertex*3 + 1] = -height /2.0f;
        v[vertex*3 + 2] = radius * cos( i * delta);

        normalB[vertex*3 + 0] = sin(i * delta);
        normalB[vertex*3 + 1] = 0;
        normalB[vertex*3 + 2] = cos(i * delta);

        // triangle 2
        vertex++;
        v[vertex*3 + 0] = radius * sin( (i+1) * delta);
        v[vertex*3 + 1] = -height /2.0f;
        v[vertex*3 + 2] = radius * cos( (i+1) * delta);

        normalB[vertex*3 + 0] = sin( (i+1) * delta);
        normalB[vertex*3 + 1] = 0;
        normalB[vertex*3 + 2] = cos( (i+1) * delta);

        vertex++;
        v[vertex*3 + 0] = radius * sin( (i+1) * delta);
        v[vertex*3 + 1] = height /2.0f;
        v[vertex*3 + 2] = radius * cos( (i+1) * delta);

        normalB[vertex*3 + 0] = sin( (i+1) * delta);
        normalB[vertex*3 + 1] = 0;
        normalB[vertex*3 + 2] = cos( (i+1) * delta);

        vertex++;
        v[vertex*3 + 0] = radius * sin( i * delta);
        v[vertex*3 + 1] = -height /2.0f;
        v[vertex*3 + 2] = radius * cos( i * delta);

        normalB[vertex*3 + 0] = sin(i * delta);
        normalB[vertex*3 + 1] = 0;
        normalB[vertex*3 + 2] = cos(i * delta);

        // base
        // central point
        vertex++;
        v[vertex*3 + 0] = 0.0f;
        v[vertex*3 + 1] = -height /2.0f;
        v[vertex*3 + 2] = 0.0f;

        normalB[vertex * 3 + 0] = 0.0f;
        normalB[vertex * 3 + 1] = -1.0f;
        normalB[vertex * 3 + 2] = 0.0f;

        vertex++;
        v[vertex*3 + 0] = radius * sin( (i+1) * delta);
        v[vertex*3 + 1] = -height /2.0f;
        v[vertex*3 + 2] = radius * cos( (i+1) * delta);

        normalB[vertex * 3 + 0] = 0.0f;
        normalB[vertex * 3 + 1] = -1.0f;
        normalB[vertex * 3 + 2] = 0.0f;

        vertex++;
        v[vertex*3 + 0] = radius * sin( i * delta);
        v[vertex*3 + 1] = -height /2.0f;
        v[vertex*3 + 2] = radius * cos( i * delta);

        normalB[vertex * 3 + 0] = 0.0f;
        normalB[vertex * 3 + 1] = -1.0f;
        normalB[vertex * 3 + 2] = 0.0f;

        vertex++;
    }

    vertexCount = vertex;
    // create two buffers
    glGenBuffers(2, buffers);
    // bind and copy data
    //   1- vertices
    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, v, GL_STATIC_DRAW);

    //   2- normais
    glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * 3, normalB, GL_STATIC_DRAW);

    free(v);
    free(normalB);

}


void drawCilinder() {

    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
    glVertexPointer(3,GL_FLOAT,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
    // normals always have 3 components
    glNormalPointer(GL_FLOAT,0,0);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


void renderScene(void) {

    // posicao luz, ultimo valor define se trata de um vetor(luz direcional)(0), ou um ponto (luz pontual/holofote)(1)
    float pos[4] = {1.0, 1.0, 1.0, 0.0};

    // material : variaveis
    float white[] = { 0.8, 0.8, 0.8, 1.0 };
    //float dark[] = { 0.2, 0.2, 0.2, 1.0 };
    //float red[] = { 0.8, 0.2, 0.2, 1.0 };

    float fps;
    int time;
    char s[64];

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(camX,camY,camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    // light position
    glLightfv(GL_LIGHT0,GL_POSITION, pos);

    // material : Os materiais são como cores, mas mais configuráveis, permite simular absorções/reflexões do material a determinada luz.
    //estamos a usar a mesma cor para ambiente e difuso porque a "luz ambiente" já está escura
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 128);


    drawCilinder();

    frame++;
    time=glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
        sprintf(s, "FPS: %f6.2", fps);
        glutSetWindowTitle(s);
    }

// End of frame
    glutSwapBuffers();
}



void processKeys(int key, int xx, int yy)
{
    switch(key) {

        case GLUT_KEY_RIGHT:
                        alfa -=0.1; break;

        case GLUT_KEY_LEFT:
                        alfa += 0.1; break;

        case GLUT_KEY_UP :
                        beta += 0.1f;
                        if (beta > 1.5f)
                            beta = 1.5f;
                        break;

        case GLUT_KEY_DOWN:
                        beta -= 0.1f;
                        if (beta < -1.5f)
                            beta = -1.5f;
                        break;

        case GLUT_KEY_PAGE_DOWN : radius -= 0.1f;
            if (radius < 0.1f)
                radius = 0.1f;
            break;

        case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

    }
    sphericalToCartesian();

}



void initGL() {

    GLfloat dark[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};

// OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

// init
    sphericalToCartesian();
    //Enable Buffers
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //Ligar iluminacao.
    glEnable(GL_LIGHTING);
    //Usando uma única fonte de luz. Se forem necessárias mais fontes de luz, é necessário acender todas elas.
    //i = [0..7]
    glEnable(GL_LIGHT0);

    //Para além da componente difusa, todos os objectos reflectem no mínimo esta iluminação ambiente uniforme
    glLightfv(GL_LIGHT0, GL_AMBIENT, dark);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    //Mancha/luz especular (Valores default: https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml)
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    prepareCilinder(2,1,16);
}


int main(int argc, char **argv) {

// init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("CG@DI-UM");

// callback registration
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

// keyboard callback registration
    glutSpecialFunc(processKeys);

#ifndef __APPLE__
// init GLEW
    glewInit();
#endif


    initGL();

    glutMainLoop();

    return 1;
}

