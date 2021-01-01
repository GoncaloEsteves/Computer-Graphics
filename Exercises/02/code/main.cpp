#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

/*
 Mover a pirâmide no plano XZ					-> Setas
 Rodar a pirâmide em torno de Y					-> Teclas 'A' e 'D'
 Rodar a pirâmide em torno de X					-> Teclas 'W' e 'S'
 Fazer escala na altura da pirâmide (eixo Y)	-> Teclas 'I' e 'K'
 Fazer escala na base da pirâmide (eixos X e Z)	-> Teclas 'J' e 'L'
 Fazer Reset ao estado da pirâmide (original)	-> Tecla 'R'

 Nota: As variáveis angleZ e translateY não estão a ser utilizadas
*/

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
    glBegin(GL_TRIANGLES);
        //Frente
        glColor3f(0.6, 0.3, 0);
        glVertex3f(0, 3, 0);
        glVertex3f(-1, 0, 1);
        glVertex3f(1, 0, 1);

        //Esquerda
        glColor3f(0, 0.5, 0.7);
        glVertex3f(0, 3, 0);
        glVertex3f(-1, 0, -1);
        glVertex3f(-1, 0, 1);

        //Direita
        glColor3f(0.2, 0, 0.5);
        glVertex3f(0, 3, 0);
        glVertex3f(1, 0, 1);
        glVertex3f(1, 0, -1);

        //Tras
        glColor3f(1, 0.3, 0.1);
        glVertex3f(0, 3, 0);
        glVertex3f(1, 0, -1);
        glVertex3f(-1, 0, -1);

        //Baixo
        glColor3f(0.4, 0.1, 1);
        glVertex3f(-1, 0, 1);
        glVertex3f(-1, 0, -1);
        glVertex3f(1, 0, -1);

        glVertex3f(1, 0, -1);
        glVertex3f(1, 0, 1);
        glVertex3f(-1, 0, 1);

    glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events

void specialKeysFunc(int key_code, int x, int y){
    switch(key_code){
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

void keysFunc(unsigned char key, int x, int y){
    switch(key){
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




int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
	glClearColor(0, 0, 0, 0);
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
    glutSpecialFunc(specialKeysFunc);
    glutKeyboardFunc(keysFunc);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
