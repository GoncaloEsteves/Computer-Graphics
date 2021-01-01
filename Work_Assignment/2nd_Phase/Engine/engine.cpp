#include <stdlib.h>

#include "engine.h"
#include "tinyxml2.h"
#include "Point.h"
#include "parser.h"

using namespace std;
using namespace tinyxml2;

void drawScene(Group *scene){
    const char* type;

    glPushMatrix();
    glColor3f(0.5f, 0.5f, 1.0f);

    for(Transformation *t : scene -> getTrans()){
        type = t -> getType().c_str();

        if(!strcmp(type, "translation"))
            glTranslatef(t -> getX(), t -> getY(), t -> getZ());

        else if(!strcmp(type, "rotation"))
            glRotatef(t -> getAngle(), t -> getX(), t -> getY(), t -> getZ());

        else if(!strcmp(type, "scale"))
            glScalef(t -> getX(), t -> getY(), t -> getZ());

        else if(!strcmp(type, "colour"))
            glColor3f(t -> getX(), t -> getY(), t -> getZ());
    }

    glBegin(GL_TRIANGLES);

    for(Shape *shape : scene -> getShapes()){
        for(Point *p : shape -> getPoints())
            glVertex3f(p -> getX(), p -> getY(), p -> getZ());
    }

    glEnd();

    for(Group *g : scene -> getGroups())
        drawScene(g);

    glPopMatrix();
}

void drawOrbits(){
    glColor3f(1.0f, 1.0f, 0.94f);

    for(auto const& p : orbits){
        glBegin(GL_POINTS);

        for(int j = 0; j < 200; j++){
            float x = p -> getX() * p -> getX();
            float y = p -> getY() * p -> getY();
            float z = p -> getZ() * p -> getZ();
            float radius = sqrtf(x + y + z);
            float alpha = (float)j * 2 * (float)(M_PI / 200);
            glVertex3f(radius * cos(alpha), 0, radius * sin(alpha));
        }

        glEnd();
    }
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

        default:{
            Point *p = orbits.at(option - 2);
            camera -> changeLook(p -> getX(), p -> getY(), p -> getZ());
            break;
        }
    }

    glutPostRedisplay();
}

void showMenu(){
    int planet = glutCreateMenu(processMenu);

    glutAddMenuEntry("Sol",1);

    for (int op = 0; op < (int)orbits.size(); op++){
        char str[10];
        sprintf(str, "Planeta %d", op+1);
        glutAddMenuEntry(str, op+2);
    }

    glutCreateMenu(processMenu);
    glutAddSubMenu("Planeta ", planet);
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

void renderScene(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camera -> getPosX(), camera -> getPosY(), camera -> getPosZ(), camera -> getLookX(), camera -> getLookY(), camera -> getLookZ(), 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, line);
    drawScene(scene);
    drawOrbits();

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

    if(argc < 2){
        cout << "Input inválido, use -h caso queira ajuda." << endl;
        return 0;
    }
    else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help")){
        MenuAjuda();
        return 0;
    }

    scene = loadXMLFile(argv[1], &orbits);
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

    glutMainLoop();

    return 1;
}