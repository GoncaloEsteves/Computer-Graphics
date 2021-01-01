#include <stdlib.h>

#include "engine.h"
#include "tinyxml2.h"
#include "Point.h"
#include "parser.h"
#include "Transformation.h"

using namespace std;
using namespace tinyxml2;


void drawOrbits(Transformation *t){
    vector<Point*> points = t -> getPointsCurve();

    glColor3f(1.0f, 1.0f, 0.94f);
    glBegin(GL_POINTS);

    for(Point *p : points){
        float x = p -> getX();
        float y = p -> getY();
        float z = p -> getZ();
        glVertex3f(x, y, z);
    }

    glEnd();
}

void transApplication(Transformation *t){
    float cTimeAux = glutGet(GLUT_ELAPSED_TIME);
    if(stop != 1)
        eTime += cTimeAux - cTime;
    cTime = cTimeAux;

    const char* type = t -> getType().c_str();
    float x = t -> getX();
    float y = t -> getY();
    float z = t -> getZ();
    float angle = t -> getAngle();
    float time = t -> getTime();

    if (strcmp(type,"translate") == 0)
        glTranslatef(x, y, z);

    else if (strcmp(type,"scale") == 0)
        glScalef(x, y, z);

    else if (strcmp(type,"rotate") == 0)
        glRotatef(angle, x, y, z);

    else if (strcmp(type,"colour") == 0)
        glColor3f(x, y, z);

    else if (strcmp(type,"rotateTime") == 0) {
        float aux = eTime * angle;
        glRotatef(aux, x, y, z);
    }

    else if (strcmp(type,"translateTime") == 0) {
        float p[4], d[4];
        float dTime = eTime *time;

        t -> getGlobalCatmullRomPoint(dTime, p, d);
        drawOrbits(t);
        glTranslatef(p[0], p[1], p[2]);

        if(t -> getDeriv()){
            float res[4];
            t -> normalize(d);
            t -> cross(d, t -> getVetor(), res);
            t -> normalize(res);
            t -> cross(res, d, t -> getVetor());
            float matrix[16];
            t -> normalize(t -> getVetor());
            t -> rotMatrix(matrix, d, t -> getVetor(), res);

            glMultMatrixf(matrix);
        }
    }
}

void drawScene(Group *scene){

    glPushMatrix();

    for(Transformation *t : scene -> getTrans()){
        transApplication(t);
    }

    vector<Shape*> shapes = scene -> getShapes();
    for(vector<Shape*> :: iterator s_it = shapes.begin(); s_it != shapes.end(); ++s_it){
        (*s_it) -> draw();
    }

    for(Group *g : scene -> getGroups())
        drawScene(g);

    glPopMatrix();
}

void MenuAjuda(){
    cout << "                               AJUDA                              " << endl;
    cout << "  Como utilizar: ./engine {ficheiro XML}                          " << endl;
    cout << "                  [-h]                                            " << endl;
    cout << "  Ficheiro:                                                       " << endl;
    cout << "  Especifique o path para o ficheiro XML onde está guardada a     " << endl;
    cout << "  informação relativa aos modelos que deseja criar.               " << endl;
    cout << "                                                                  " << endl;
    cout << "    ↑ : Rodar a vista para cima                                   " << endl;
    cout << "                                                                  " << endl;
    cout << "    ↓ : Rodar a vista para baixo                                  " << endl;
    cout << "                                                                  " << endl;
    cout << "    ← : Rodar a vista para a esquerda                             " << endl;
    cout << "                                                                  " << endl;
    cout << "    → : Rodar a vista para a direita                              " << endl;
    cout << "                                                                  " << endl;
    cout << "    F1 : Aumentar tamanho da imagem                               " << endl;
    cout << "                                                                  " << endl;
    cout << "    F2 : Diminuir tamanho da imagem                               " << endl;
    cout << "                                                                  " << endl;
    cout << "    F6 : Estado inicial                                           " << endl;
    cout << "                                                                  " << endl;
    cout << "    Formato:                                                      " << endl;
    cout << "    F3: Preencher a figura                                        " << endl;
    cout << "                                                                  " << endl;
    cout << "    F4: Mudar a figura para linhas apenas                         " << endl;
    cout << "                                                                  " << endl;
    cout << "    F5: Mudar a figura para pontos apenas                         " << endl;
    cout << "                                                                  " << endl;
}

void processMenu(int option){
    switch (option){
        case 0:
            exit(0);

        case 1:
            camera -> posIniCamera();
            break;

        case 2:
            stop = 0;
            break;

        case 3:
            stop = 1;
            break;

        default:
            break;
    }

    glutPostRedisplay();
}

void showMenu(){

    glutAddMenuEntry("Sol",1);
    int planets = glutCreateMenu(processMenu);
    glutAddMenuEntry("Ligar",2);
    glutAddMenuEntry("Desligar",3);
    glutCreateMenu(processMenu);
    glutAddSubMenu("Movimento", planets);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void specialKeys(int key, int a, int b){
    (void) a;
    (void) b;

    switch (key){
        case GLUT_KEY_F3:
            line = GL_FILL;
            break;

        case GLUT_KEY_F4:
            line = GL_LINE;
            break;

        case GLUT_KEY_F5:
            line = GL_POINT;
            break;

        default:
            camera -> specialKeysCamera(key);
            break;
    }

    glutPostRedisplay();
}

void pressMouse(int button, int state, int x, int y){
    camera -> pressMouse(button, state, x, y);
}

void motionMouse(int x, int y){
    camera -> motionMouse(x, y);
}

void fps(){
    int time;
    char name[30];

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);

    if(time - timebase > 1000){
        float f = frame * 1000/(time - timebase);
        timebase = time;
        frame = 0;
        sprintf(name, "Sistema Solar %.2f FPS", f);
        glutSetWindowTitle(name);
    }
}

void renderScene(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camera -> getPosX(), camera -> getPosY(), camera -> getPosZ(), camera -> getLookX(), camera -> getLookY(), camera -> getLookZ(), 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, line);
    fps();
    drawScene(scene);

    glutSwapBuffers();
}

void changeSize(int w, int h){

    if(h == 0)
        h = 1;

    float ratio = (float)w * 1.0f / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Sistema_Solar");

    #ifndef __APPLE__
        if (glewInit() != GLEW_OK){
            cout << "Problema com o GLEW." << endl;
            return -1;
        }
    #endif

    if(argc < 2){
        cout << "Input inválido, use -h caso queira ajuda." << endl;
        return 0;
    }
    else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help")){
        MenuAjuda();
        return 0;
    }

    scene = loadXMLFile(argv[1]);
    camera = new Camera();
    if(scene == nullptr)
        return 0;

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutSpecialFunc(specialKeys);

    glutMouseFunc(pressMouse);
    glutMotionFunc(motionMouse);

    showMenu();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnableClientState(GL_VERTEX_ARRAY);

    glutMainLoop();

    return 1;
}