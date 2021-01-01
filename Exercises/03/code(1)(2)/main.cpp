#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

// variaveis de rotação
float angleX = 0;
float angleY = 0;
float angleZ = 0;

// variáveis de translação
float translateX = 0;
float translateY = 0;
float translateZ = 0;

// variáveis de escala
float scaleX = 1;
float scaleY = 1;
float scaleZ = 1;


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


void drawCylinder(float radius, float height, int slices) {
    for(int i=0; i<slices; i++) {
        float theta = (((float)i)/(float)slices)*(2.0*M_PI);
        float nextTheta = (((float)i+1.0)/(float)slices)*(2.0*M_PI);
        glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0, 0, 0.0); //A
        glVertex3f(radius*cos(theta), 0, radius*sin(theta)); //c
        glVertex3f(radius*cos(nextTheta), 0, radius*sin(nextTheta)); //e

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0, height, 0.0); //b
        glVertex3f (radius*cos(nextTheta), height, radius*sin(nextTheta)); //f
        glVertex3f(radius*cos(theta), height, radius*sin(theta)); //d

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(radius*cos(theta), 0, radius*sin(theta)); //c
        glVertex3f(radius*cos(theta), height, radius*sin(theta)); //d
        glVertex3f (radius*cos(nextTheta), height, radius*sin(nextTheta)); //f

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f (radius*cos(nextTheta), height, radius*sin(nextTheta)); //f
        glVertex3f(radius*cos(nextTheta), 0, radius*sin(nextTheta)); //e
        glVertex3f(radius*cos(theta), 0, radius*sin(theta)); //c

        glEnd();
    }
}


void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(5.0,5.0,5.0,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);
    glBegin(GL_LINES);
    //Eixo X
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(15, 0, 0);

    //Eixo Y
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 15, 0);

    //Eixo Z
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 15);
    glEnd();

    // put the geometric transformations here
    glScalef(scaleX, scaleY, scaleZ);
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    // put drawing instructions here
    drawCylinder(1,2,10);

    glEnd();

    // End of frame
    glutSwapBuffers();
}



void processKeys(unsigned char c, int xx, int yy) {
    switch(c){
        // Rotacoes nos eixos Y (a, d) e X (w, s)
        case 'a':
            angleY -= 10;
            break;
        case 'd':
            angleY += 10;
            break;
        case 'w':
            angleX -= 10;
            break;
        case 's':
            angleX += 10;
            break;
            // Escala Altura
        case 'i':
            scaleY += 0.1;
            break;
        case 'k':
            scaleY -= 0.1;
            break;
            // Escala Base
        case 'j':
            scaleX -= 0.1;
            scaleZ -= 0.1;
            break;
        case 'l':
            scaleX += 0.1;
            scaleZ += 0.1;
            break;
            // reset ao estado inicial
        case 'r':
            angleX = 0;
            angleY = 0;
            angleZ = 0;
            translateX = 0;
            translateY = 0;
            translateZ = 0;
            scaleX = 1;
            scaleY = 1;
            scaleZ = 1;
        default:
            break;
    }
    glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {
    switch(key){
        case GLUT_KEY_RIGHT:
            translateX += 1;
            break;
        case GLUT_KEY_LEFT:
            translateX -= 1;
            break;
        case GLUT_KEY_UP:
            translateZ -= 1;
            break;
        case GLUT_KEY_DOWN:
            translateZ += 1;
            break;
        default:
            break;
    }
    glutPostRedisplay();
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
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
