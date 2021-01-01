#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

float * trees;

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
int nTrees = 200;
int nTeapot = 8;
int radiusTeaInt = 15;
int radiusTeaExt = 35;
float radiusBig = 50;
float angle = 0;


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

void drawTrees(int n){
    int i, l = 0;
    float x, z;

    for(i = 0; i < n; i++){
        x = trees[l++];
        z = trees[l++];

        glPushMatrix();
        glTranslatef(x, 0, z);
        glRotatef(-90, 1, 0, 0);

        glColor3f(0.48627,0.21176,0.1490);
        glutSolidCone(1, 3, 8, 4);

        glTranslatef(0,0,2.5);
        glColor3f(0.26666,0.4,0.1333);
        glutSolidCone(3, 5, 8, 4);

        glPopMatrix();
    }
}

void drawTeapotsInt(int n){
    int i;
    float alpha;
    glColor3f(0, 0, 1);

    for(i = 0; i < n; i++){
        alpha = i * (360/n);

        glPushMatrix();
        glRotatef(angle + alpha, 0, 1, 0);
        glTranslatef(radiusTeaInt, 3, 0);
        glutSolidTeapot(3);
        glPopMatrix();
    }

    angle += 0.5;
}

void drawTeapotsExt(int n){
    int i;
    float alpha;
    glColor3f(1, 0, 0);

    for(i = 0; i < n*2; i++){
        alpha = i * (360/(n*2));

        glPushMatrix();
        glRotatef(angle + alpha, 0, 1, 0);
        glTranslatef(radiusTeaExt, 3, 0);
        glRotatef(90, 0, 1, 0);
        glutSolidTeapot(3);
        glPopMatrix();
    }

    angle += 0.5;
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();

	drawTrees(nTrees);
	drawTeapotsInt(nTeapot);
	drawTeapotsExt(nTeapot);

	glColor3f(1, 0, 0.5);
    glutSolidTorus(2,3,20,20);

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

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
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
    glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	buildTrees(nTrees);
	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
