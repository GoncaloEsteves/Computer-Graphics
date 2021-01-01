#include <stdlib.h>
#include "tinyxml2.h"
#include "engine.h"

using namespace std;
using namespace tinyxml2;

void drawAxes() {
    glBegin(GL_LINES);

    // Eixo X (vermelho)
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(15, 0, 0);

    // Eixo Y (azul)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 15, 0);

    // Eixo Z (verde)
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 15);

    glEnd();
}

void drawPrimitives(void){

    glBegin(GL_TRIANGLES);
    int i = 0;
    bool cor = true;

    for(const Point pt : points){
        if(i == 3){
            cor = !cor;
            i = 0;
        }

        if(cor){
            glColor3f(0.2, 0.2, 1);
            glVertex3f(pt.x, pt.y, pt.z);
        }
        else{
            glColor3f(0.7, 0.7, 1);
            glVertex3f(pt.x, pt.y, pt.z);
        }

        i++;
    }
    glEnd();
}

int readPointsFile(string filename){

    Point p;
    string l, aux;
    ifstream file(filename);
    int i;

    if(!file.is_open()){
        cout << "Impossível abrir ficheiro: " << filename << "." << endl;
        return -1;
    }
    else{
        while(!file.eof()){
            getline(file, l);
            stringstream ss(l.c_str());

            if(l.c_str() != NULL){
                for(i = 0; getline(ss, aux, ','); i++){
                    if(i == 0)
                        p.x = stof(aux);
                    else if(i == 1)
                        p.y = stof(aux);
                    else
                        p.z = stof(aux);
                }
                points.push_back(p);
            }
        }
        points.pop_back();
        file.close();
        return 0;
    }
}

int loadXMLfile(string filename){

    XMLDocument xmlDoc;
    XMLNode *pNode;
    XMLElement *pElement, *pListElement;
    string fileDir = "../../XMLs/" + filename;
    XMLError error = xmlDoc.LoadFile(fileDir.c_str());

    if(error == XML_SUCCESS){
        pNode = xmlDoc.FirstChild();

        if(pNode != nullptr){
            pElement = pNode -> FirstChildElement("models");

            if(pElement != nullptr){
                pListElement = pElement -> FirstChildElement("model");

                while(pListElement != nullptr){
                    string file;
                    file = pListElement -> Attribute("file");

                    if(!file.empty() && readPointsFile(file) == -1)
                        return -1;

                    pListElement = pListElement -> NextSiblingElement("model");
                }
            }
        }
    }

    else{
        cout << "Impossível abrir ficheiro: " << filename << "." << endl;
        return -1;
    }

    return 0;
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
    cout << "    Formato:                                                      " << endl;
    cout << "    F3: Preencher a figura                                        " << endl;
    cout << "                                                                  " << endl;
    cout << "    F4: Mudar a figura para linhas apenas                         " << endl;
    cout << "                                                                  " << endl;
    cout << "    F5: Mudar a figura para pontos apenas                         " << endl;
    cout << "                                                                  " << endl;
}

void specialKeys(int key, int a, int b){

    (void) a;
    (void) b;

    switch (key){
        case GLUT_KEY_UP:
            if(beta < (M_PI/2 - step))
                beta += step;
            break;

        case GLUT_KEY_DOWN:
            if(beta > -(M_PI/2 - step))
                beta -= step;
            break;

        case GLUT_KEY_LEFT:
            alpha -= step;
            break;

        case GLUT_KEY_RIGHT:
            alpha += step;
            break;

        case GLUT_KEY_F1:
            radius -= step;
            break;

        case GLUT_KEY_F2:
            radius += step;
            break;

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
            break;
    }

    glutPostRedisplay();
}

void renderScene(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha), 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, line);
    drawAxes();
    drawPrimitives();

    glutSwapBuffers();
}

void changeSize(int w, int h){

    if(h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

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
    else if (loadXMLfile(argv[1]) == 0){
        glutDisplayFunc(renderScene);
        glutReshapeFunc(changeSize);
        glutIdleFunc(renderScene);
        glutSpecialFunc(specialKeys);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();
}