// Computación Gráfica Cubo.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <SOIL2/SOIL2.h>
#include <sstream>
#include <time.h>

#include "glm/vec3.hpp" // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp>

using namespace std;


typedef struct {
    GLfloat vertexXYZ[3];
    GLfloat colorRGB[3];
} Vertex;


typedef struct {
    float x, y;
    int left, right;
} MouseState;

int window[2];
GLfloat formato_global;
MouseState mouse;
int square_selected;
int vertex_selected;
bool move_vertex = false;
bool move_light_point = false;
bool add_texture = false;
bool show_galaxy = false;
bool* keyStates = new bool[256];
int timer = 0;

//string pathImage = "/Users/jpcas/Downloads/textures/";
string pathImage = "./textures/";
int cantImages = 30;

// Red Square
Vertex square_1[4] = {
    {{-3,4,0},{1,0,0}},
    {{-3,2,0},{1,0,0}},
    {{-1,2,0},{1,0,0}},
    {{-1,4,0},{1,0,0}}
};
// Green Square
Vertex square_2[4] = {
    {{-9,4,0},{0,1,0}},
    {{-9,2,0},{0,1,0}},
    {{-7,2,0},{0,1,0}},
    {{-7,4,0},{0,1,0}}
};
// Blue Square
Vertex square_3[4] = {
    {{-6,4,0},{0,0,1}},
    {{-6,2,0},{0,0,1}},
    {{-4,2,0},{0,0,1}},
    {{-4,4,0},{0,0,1}}
};


Vertex sun_square[4] = {
    {{ 2,6,0},{0,0,1}},
    {{ 2,4,0},{0,0,1}},
    {{-2,4,0},{0,0,1}},
    {{-2,6,0},{0,0,1}}
};

const int numArms = 5;
const float PI = 3.14;
const float armSeparationDistance = 2 * PI / numArms;
const float armOffsetMax = 0.5f;
const float rotationFactor = 5;
const float randomOffsetXY = 0.02f;

const int numberStars = 10000;
Vertex starPositions[numberStars];

GLfloat lightPosition[] = { 0.0, 8.0, 1.0, 1.0};

float RandFloat() {
    return ((float)rand() / (float)RAND_MAX);
}


void InitializeStars() {
    for (int i = 0; i < numberStars; i++) {
        // Choose a distance from the center of the galaxy.
        float distance = RandFloat();
        distance = pow(distance, 2);

        // Choose an angle between 0 and 2 * PI.
        float angle = RandFloat() * 2 * PI;
        float armOffset = RandFloat() * armOffsetMax;
        armOffset = armOffset - armOffsetMax / 2;
        armOffset = armOffset * (1 / distance);

        float squaredArmOffset = pow(armOffset, 2);
        if (armOffset < 0)
            squaredArmOffset = squaredArmOffset * -1;
        armOffset = squaredArmOffset;

        float rotation = distance * rotationFactor;

        angle = (int)(angle / armSeparationDistance) * armSeparationDistance + armOffset + rotation;

        // Convert polar coordinates to 2D cartesian coordinates.
        GLfloat starX = cos(angle) * distance;
        GLfloat starY = sin(angle) * distance;

        float randomOffsetX = RandFloat() * randomOffsetXY;
        float randomOffsetY = RandFloat() * randomOffsetXY;

        starX += randomOffsetX;
        starY += randomOffsetY;

        // Now we can assign xy coords.
        GLfloat v[2][3] = { { starX, starY, 0.0 }, {1.0, 1.0, 1.0 } };
        starPositions[i] = { { starX, starY, 0.0 }, {1.0, 1.0, 1.0 } };
        /*starPositions[i].x = starX;
        starPositions[i].y = starY;*/
    }
}


void displayGalaxy() {
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);

    timer = 0;

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < numberStars; i++) {
        glColor3fv(starPositions[i].colorRGB);
        glVertex3fv(starPositions[i].vertexXYZ);
    }
    glEnd();
}


void keyPressed(unsigned char key, int x, int y) {
    keyStates[key] = true;
}


void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;

    switch (key) {
    case 'f':
        glutFullScreen();
        break;
    case 's':
        glutReshapeWindow(800, 600);
        glutPositionWindow(100, 100);
        break;
    case 'e':
        exit(1);
    case 't':
        add_texture = true;
        break;
    case 'r':
        add_texture = false;
        break;
    case 'g':
        show_galaxy = true;
    }
    
}


void InitGLUT() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Proyecto Computacion Grafica");
}


void Reshape(int w, int h) {
    GLfloat formato;

    window[0] = w;
    window[1] = h;

    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    formato = (GLfloat)w / (GLfloat)h;
    formato_global = formato;
    if (w <= h) glOrtho(-10.0f, 10.0f, -10.0f / formato, 10.0f / formato, -1.0f, 1.0f);
    else glOrtho(-10.0f * formato, 10.0f * formato, -10.0f, 10.0f, -1.0f, 1.0f);
}


void toWorldCoords(float x, float y, float &wx, float &wy, float x_min, float x_max, float y_min, float y_max) {
    int w = window[0];
    int h = window[1];
    
    float relative_x = ((float)x / w);
    float relative_y = ((float)y / h);

    GLfloat format = (GLfloat)w / (GLfloat)h;


    if (w <= h) {
        wx = (x_min + ((x_max - x_min) * relative_x));
        wy = -((y_min / format) + ((y_max - y_min) / format) * relative_y);
    }
    else {
        wx = ((x_min * format) + ((x_max - x_min) * format) * relative_x);
        wy = -(y_min + ((y_max - y_min) * relative_y));
    }
}


void MoveVertex(int val){
    int square = square_selected;
    int vertex = vertex_selected;
    float x = mouse.x;
    float y = mouse.y;

    if (move_light_point) {
        lightPosition[0] = x;
        lightPosition[1] = y;
    }
    
    else if (move_vertex) {

        cout << "Square selected: " << square_selected << endl;
        cout << "Vertex selected: " << vertex_selected << endl;

        if (square == 1) {
            square_1[vertex].vertexXYZ[0] = x;
            square_1[vertex].vertexXYZ[1] = y;
        }
        else if (square == 2) {
            square_2[vertex].vertexXYZ[0] = x;
            square_2[vertex].vertexXYZ[1] = y;
        }
        else {
            square_3[vertex].vertexXYZ[0] = x;
            square_3[vertex].vertexXYZ[1] = y;
        }
    }
    
    glutTimerFunc(33, MoveVertex, 1);
    glutPostRedisplay();
}


void MovementMouse(int x, int y) {
    float wx, wy;
    toWorldCoords(x, y, wx, wy, -10, 10, -10, 10);
    mouse.x = wx;
    mouse.y = wy;
}


void MouseControl(int button, int state, int x, int y) {
    
    int w = window[0];
    int h = window[1];

    float wx;
    float wy;

    toWorldCoords(x, y, wx, wy, -10, 10, -10, 10);

    mouse.x = wx;
    mouse.y = wy;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        for (int i = 0; i < 4; ++i) {
            if (wx > square_1[i].vertexXYZ[0] - 0.5f &&
                wx < square_1[i].vertexXYZ[0] + 0.5f &&
                wy > square_1[i].vertexXYZ[1] - 0.5f &&
                wy < square_1[i].vertexXYZ[1] + 0.5f) {
                
                vertex_selected = i;
                square_selected = 1;
                move_vertex = true;
            }
        }

        for (int i = 0; i < 4; ++i) {
            if (wx > square_2[i].vertexXYZ[0] - 0.5f &&
                wx < square_2[i].vertexXYZ[0] + 0.5f &&
                wy > square_2[i].vertexXYZ[1] - 0.5f &&
                wy < square_2[i].vertexXYZ[1] + 0.5f) {

                vertex_selected = i;
                square_selected = 2;
                move_vertex = true;
            }
        }

        for (int i = 0; i < 4; ++i) {
            if (wx > square_3[i].vertexXYZ[0] - 0.5f &&
                wx < square_3[i].vertexXYZ[0] + 0.5f &&
                wy > square_3[i].vertexXYZ[1] - 0.5f &&
                wy < square_3[i].vertexXYZ[1] + 0.5f) {

                vertex_selected = i;
                square_selected = 3;
                move_vertex = true;
            }
        }
        if (wx > lightPosition[0] - 1.5f &&
            wx < lightPosition[0] + 1.5f &&
            wy > lightPosition[1] - 1.5f &&
            wy < lightPosition[1] + 1.5f) {

            move_light_point = true;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && move_vertex) {

        cout << "Move vertex !" << endl;   
        move_vertex = false;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && move_light_point) {
        move_light_point = false;
    }
    cout << "x: " << x << " y:" << y << endl;
}


string convertInt(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}


GLuint loadTexture(const char* filename) {
    GLuint textureID;
    
    textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO, 
        SOIL_CREATE_NEW_ID, 
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );


    return textureID;
}


vector<GLuint> square_1_textures;
vector<GLuint> square_2_textures;
vector<GLuint> square_3_textures;
vector<GLuint> sun_textures;
    
void loadTextures() {

    for (int i = 0; i < cantImages; i++) {
        string filename_field = pathImage + "field-" + convertInt(i) + ".jpg";
        string filename_sky = pathImage + "sky-" + convertInt(i) + ".jpg";
        GLuint texture_2d_field = loadTexture(filename_field.c_str());
        GLuint texture_2d_sky = loadTexture(filename_sky.c_str());
        
        square_1_textures.push_back(texture_2d_field);
        square_2_textures.push_back(texture_2d_field);
        square_3_textures.push_back(texture_2d_sky);
        
        cout << "i: " << i << endl;
    }
}


void addTextures() {
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);
    glLoadIdentity();
    //gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)

    glEnable(GL_TEXTURE_2D);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat diffusedLight[] = { 6.0, 6.0, 6.0, 0.0 };
    GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    //GLfloat lightPosition[] = { -1.0, -1.0, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);

    timer++;

    glColor3f(1.0f, 1.0f, 1.0f);
    
    cout << "Inicio de asignacion de texturas" << endl;
    // Textures of square 1 (RED)
    glBindTexture(GL_TEXTURE_2D, square_1_textures[timer%cantImages]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    //glNormal3f(0.0, 0.0, 1.0);
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            glTexCoord2f(1.0, 1.0);
        }
        else if (i == 1) {
            glTexCoord2f(1.0, 0.0);
        }
        else if (i == 2) {
            glTexCoord2f(0.0, 0.0);
        }
        else if (i == 3) {
            glTexCoord2f(0.0, 1.0);
        }

        glVertex3fv(square_1[i].vertexXYZ);
    }
    glEnd();

    cout << "Termino de asignar texturas Square 1" << endl;
    // Textures of square 2 (GREEN)
    glBindTexture(GL_TEXTURE_2D, square_2_textures[timer % cantImages]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    //glNormal3f(0.0, 0.0, 1.0);
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            glTexCoord2f(1.0, 1.0);
        }
        else if (i == 1) {
            glTexCoord2f(1.0, 0.0);
        }
        else if (i == 2) {
            glTexCoord2f(0.0, 0.0);
        }
        else if (i == 3) {
            glTexCoord2f(0.0, 1.0);
        }

        glVertex3fv(square_2[i].vertexXYZ);
    }
    glEnd();
    cout << "Termino de asignar texturas Square 2" << endl;

    // Textures of square 3 (BLUE)
    glBindTexture(GL_TEXTURE_2D, square_3_textures[timer % cantImages]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    //glNormal3f(0.0, 0.0, 1.0);
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            glTexCoord2f(1.0, 1.0);
        }
        else if (i == 1) {
            glTexCoord2f(1.0, 0.0);
        }
        else if (i == 2) {
            glTexCoord2f(0.0, 0.0);
        }
        else if (i == 3) {
            glTexCoord2f(0.0, 1.0);
        }

        glVertex3fv(square_3[i].vertexXYZ);
    }
    glEnd();

    // Textures of sun
    GLfloat texture[] = { 1.0 ,1.0, 1.0, 1.0 };
    glBindTexture(GL_TEXTURE_2D, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPointSize(20);
    glBegin(GL_POINTS);
    //glColor3f(1.0, 1.0, 1.0);
    glVertex3f(lightPosition[0], lightPosition[1], 0.0);
    glEnd();
    cout << "Termino de asignar texturas Square 3" << endl;
    
}


void displayDefaultSquares() {
    timer = 0;
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Square 1
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; ++i) {
        glColor3fv(square_1[i].colorRGB);
        glVertex3fv(square_1[i].vertexXYZ);
    }
    glEnd();
    // Square 2
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; ++i) {
        glColor3fv(square_2[i].colorRGB);
        glVertex3fv(square_2[i].vertexXYZ);
        
    }
    glEnd();
    // Square 3
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; ++i) {
        glColor3fv(square_3[i].colorRGB);
        glVertex3fv(square_3[i].vertexXYZ);
    }
    glEnd();
}


void sleep(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}


void Render() {
    if (add_texture) {
        addTextures();
        sleep(50);
        cout << "Texture render !!" << endl;
    }
    else {
        displayDefaultSquares();
    }
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    cout << pathImage << endl;
    
    glutInit(&argc, argv);
    InitGLUT();
    loadTextures();
    InitializeStars();

    glutReshapeFunc(Reshape);
    glutDisplayFunc(Render);
    glutIdleFunc(Render);
    glutMouseFunc(MouseControl);
    glutTimerFunc(33, MoveVertex, 1);
    glutMotionFunc(MovementMouse);
    glutKeyboardFunc(keyPressed); 
    glutKeyboardUpFunc(keyUp);

    glutMainLoop();
}