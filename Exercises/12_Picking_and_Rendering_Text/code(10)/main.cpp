
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

float alpha = 0, beta = 35, r = 10;

int w,h;
char s[30];
int timebase=0;
int frame=0;

float orange[4] = {0.8f, 0.4f , 0.4f,1.0f};
float green[4] = {0.4f, 0.8f, 0.4f, 1.0f};
float black[4] = {0.0f,0.0f,0.0f,0.0f};
float white[4] = {1.0f, 1.0f , 1.0f,1.0f};
float blue[4] = { 0.0f, 0.45f, 0.8f, 1.0f };

unsigned int picked = 0;


void changeSize(int ww, int hh) {

    float ratio;

    w = ww;
    h = hh;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    ratio = 1.0f * w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the clipping volume
    gluPerspective(45,ratio,1,1000);
    glMatrixMode(GL_MODELVIEW);
}


void drawSnowMan() {

    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, white);

// Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);

// Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f,20,20);

// Draw Eyes
    glPushMatrix();
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, black);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f,10,10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f,10,10);
    glPopMatrix();

// Draw Nose
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, orange);
    glRotatef(0.0f,1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f,0.5f,10,2);
}


void drawSnowMan(int code) {

    float color = code / 255.0f;

    glColor3f(color, color, color);

// Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);


// Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f,20,20);

// Draw Eyes
    glPushMatrix();
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f,10,10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f,10,10);
    glPopMatrix();

// Draw Nose
    glRotatef(0.0f,1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f,0.5f,10,2);
}


void renderText() {

    void* font = GLUT_BITMAP_HELVETICA_18; //  Fonte da letra
    char text[64];
    //float textw;

// O nosso texto será a cor branca , vamos desactivar temporariamente a luz para não interferir na cor do texto
    glColor3f(1,1,1);
    glDisable(GL_LIGHTING);

// Guardar a projeção anterior
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

// Projecção ortogonal para que as coordenadas de desenho coincidam com o tamanho da janela em pixeis
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();

// Centrar o texto, calculando a dimensão da mensagem em pixeis
    //textw = glutBitmapLength(font, (unsigned char*) text.c_str());
    //glRasterPos2d(w/2 - textw/2, 24); // text position in pixels
    glRasterPos2d(10, 20); // text position in pixels

// Ignorar profundidade
    glDisable(GL_DEPTH_TEST);

// Seleccao do Boneco
    if(picked) {
        sprintf(text,"Picked: %d",picked);
    }

    else{
        sprintf(text,"Nothing picked");
    }

// Desenhar a mensagem, caracter a caracter
    for(char *c = text; *c != '\0'; c++){
        glutBitmapCharacter(font, *c);
    }


// Restaurar as matrizes anteriores
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}


void renderScene(void) {

    int timet;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    float dir[4] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, dir);
// Draw ground

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidTeapot(1);
    glPopMatrix();

    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, green);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();

// Draw SnowMen
    for(int i = 0; i < 2; i++)
        for(int j=0; j < 2; j++) {
            glPushMatrix();
            glTranslatef(i*5.0-2.5f,0,j * 5.0-2.5f);
            drawSnowMan();
            glPopMatrix();
        }

    renderText();

    frame++;
    timet=glutGet(GLUT_ELAPSED_TIME);
    if (timet - timebase > 1000) {
        sprintf(s,"FPS:%4.2f",frame*1000.0/(timet-timebase));
        timebase = timet;
        frame = 0;
        glutSetWindowTitle(s);
    }
    glutSwapBuffers();
}


unsigned char  picking(int x, int y) {
// parametros de entrada x e y correspondem �s coordenadas do rato na janela

    unsigned char res[4];
    GLint viewport[4];

// Desactivar iluminacao e texturas
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
/*
De seguida e necessario limpar o color buffer para podermos desenhar os bonecos de neve codificados.
Iremos limpar apenas o color buffer, e nao o depth buffer, visto que podemos usufruir dos valores de
profundidade ja presentes no depth buffer, que sao corretos dado estarmos a desenhar a mesma cena.
*/
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);
/*
Dado que nao apagamos o depth buffer, se desenhassemos a cena tal e qual (com a funcao de
comparacao default GL_LESS para o depth test) entao os bonecos de neve codificados seriam rejeitados
por se encontrarem exactamente nas mesmas posicoes dos da frame atual, como tal temos de alterar a
funcao de teste para GL_LEQUAL.
*/	
    glDepthFunc(GL_LEQUAL);
/*
Depois procedemos ao desenho da cena, tal como na funcao render scene, mas com os valores de cor
codificados por boneco-de-neve.
*/
    for(int i = 0; i < 2; i++)
        for(int j=0; j < 2; j++) {
            glPushMatrix();
            glTranslatef(i*5.0-2.5f,0,j * 5.0-2.5f);
            drawSnowMan(i*2+j+1); // i*2+j+1 => durante este ciclo os parametros para a cor serao 1;2;3;4 que vão corresponder a cada boneco
            glPopMatrix();
        }
/*
Por ultimo temos de recuperar a cor do pixel que foi clicado, podemos fazer isso recorrendo a funcao
glReadPixels, juntamente com as coordenadas do rato, obtidas pelo GLUT. No entanto em OpenGL o
eixo dos y encontra-se invertido.
*/
//Ler o pixel debaixo da posicao do rato
    glGetIntegerv(GL_VIEWPORT, viewport);
    //read a block of pixels from the frame buffer
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);

// Concluimos a abordagem reativando a texturas e luz, e devolvendo a componente R do pixel clicado
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
// Return red color component
    return res[0];
}


// ----------------------------------------------------------
//				MOUSE AND KEYBOARD
// ----------------------------------------------------------


void processNormalKeys(unsigned char key, int x, int y) {

    switch(key) {

        case 27: exit(0);
        case 'c': printf("Camera : %f %f %f\n", alpha, beta, r); break;
    }
}


void processMouseButtons(int button, int state, int xx, int yy)
{
    printf("%d %d\n", xx, yy);
    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else { // Middle button
            tracking = 0;
            picked = picking(xx,yy);
            if (picked)
                printf("Picked Snowman number %d\n", picked);
            else
                printf("Nothing selected\n");
            glutPostRedisplay();
        }
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


void processMouseMotion(int xx, int yy)
{

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
    camY = rAux * 							     sin(betaAux * 3.14 / 180.0);

    glutPostRedisplay();
}




void init() {

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    camX = r * sin(alpha * 3.14 / 180.0) * cos(beta * 3.14 / 180.0);
    camZ = r * cos(alpha * 3.14 / 180.0) * cos(beta * 3.14 / 180.0);
    camY = r * 							   sin(beta * 3.14 / 180.0);
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(512,512);
    glutCreateWindow("Picking SnowMen from CG @ DI");

// registo de fun��es
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// registo da fun��es do teclado e rato
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

    init();

    glutMainLoop();

    return 0;
}
