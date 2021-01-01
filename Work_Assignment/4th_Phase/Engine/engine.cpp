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
    float cor[4]={0.2f, 0.2f, 0.2f, 1.0f};

    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cor);
    glBegin(GL_LINE_LOOP);

    for(Point *p : points){
        float normal[3] = {-(p -> getX()), -(p -> getY()), -(p -> getZ())};
        t -> normalize(normal);
        glNormal3fv(normal);
        float x = p -> getX();
        float y = p -> getY();
        float z = p -> getZ();
        glVertex3f(x, y, z);
    }

    glEnd();
    glPopAttrib();
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

    else if (strcmp(type,"rotateTime") == 0) {
        float aux = eTime * angle;
        glRotatef(aux, x, y, z);
    }

    else if (strcmp(type,"translateTime") == 0) {
        float p[4], d[4];
        float dTime = eTime *time;

        t -> getGlobalCatmullRomPoint(dTime, p, d);
        Transformation* diffuse = nullptr;
        Transformation* ambient = nullptr;
        Transformation* specular = new Transformation(0, 0, 0);
        Transformation* emission = new Transformation(0, 0, 0);
        Material *m = new Material(diffuse, ambient, specular, emission);
        m -> draw();
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

void drawScene(Scene *sc, Group *groups){

    glPushMatrix();

    for(Transformation *t : groups -> getTrans()){
        transApplication(t);
    }

    vector<Shape*> shapes = groups -> getShapes();

    for(vector<Shape*> :: iterator s_it = shapes.begin(); s_it != shapes.end(); ++s_it){
        (*s_it) -> draw();
    }

    for(Group *g : groups -> getGroups())
        drawScene(sc, g);

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
    cout << "    ↑ : Aproximar a imagem                                        " << endl;
    cout << "                                                                  " << endl;
    cout << "    ↓ : Afastar a imagem                                          " << endl;
    cout << "                                                                  " << endl;
    cout << "    ← : Deslocar a vista para a esquerda                          " << endl;
    cout << "                                                                  " << endl;
    cout << "    → : Deslocar a vista para a direita                           " << endl;
    cout << "                                                                  " << endl;
    cout << "    F1 : Estado inicial                                           " << endl;
    cout << "                                                                  " << endl;
    cout << "    Formato:                                                      " << endl;
    cout << "    F2: Preencher a figura                                        " << endl;
    cout << "                                                                  " << endl;
    cout << "    F3: Mudar a figura para linhas apenas                         " << endl;
    cout << "                                                                  " << endl;
    cout << "    F4: Mudar a figura para pontos apenas                         " << endl;
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

        case -1:
            glEnable(GL_LIGHTING);
            break;

        case -2:
            glDisable(GL_LIGHTING);
            break;

        default:
            break;
    }

    glutPostRedisplay();
}

void showMenu(){

    glutAddMenuEntry("Posicao Inicial",1);
    int menu = glutCreateMenu(processMenu);
    glutAddMenuEntry("Ligar",2);
    glutAddMenuEntry("Desligar",3);
    glutCreateMenu(processMenu);
    glutAddSubMenu("Movimento", menu);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void specialKeys(int key, int a, int b){
    (void) a;
    (void) b;

    switch (key){
        case GLUT_KEY_F2:
            line = GL_FILL;
            break;

        case GLUT_KEY_F3:
            line = GL_LINE;
            break;

        case GLUT_KEY_F4:
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
    gluLookAt(camera -> getPosX(), camera -> getPosY(), camera -> getPosZ(), camera -> getOrX(), camera -> getOrY(), camera -> getOrZ(), 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, line);

    scene -> lightApplication();

    fps();
    drawScene(scene, scene -> getGroup());

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

void init(){
    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glEnable(GL_RESCALE_NORMAL);
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

    init();

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

    glutMainLoop();

    return 1;
}