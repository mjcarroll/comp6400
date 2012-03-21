/***
 * Homework Assignment #4
 * Michael Carroll <carromj@auburn.edu>
 */

 /*
    Requirements
    ============

    1.The window size should be at least 800x800.

    2.The window's title should be "COMP-5/6400 Assignment 4"

    3.Create a 3D environment (changing your 2-D Shelby Complex into 3-D) for
    the student you have created in assignment 3.

    4.Continue the work from assignments 1, 2 & 3 and allow the student to move
    around the environment.

    5.Use perspective projection, instead of orthographic projection.

    6.Allow the user to move the viewpoint around the environment. 

    Description
    ===========

    You can change your 2-D Shelby Complex in Assignments 1 & 2 into a 3-D
    version. There are usually many people, like students, faculty, staff, and
    visitors, etc. around the complex.  Common sense should apply here.  You do
    not need to consider object collision yet (i.e., allow an object, e.g., a
    student, to move through other objects) at this moment.  This problem will
    be solved in the next assignment.

    The design you have from assignment 3 should be enhanced.  A user should be
    able to direct at least one person, i.e., the main student character, to do
    certain maneuvers in the complex.
 
    As design tips, you may allow the user to zoom in and out the view,
    duplicate people (we are in virtual world …), etc.
 
    The design of your interface must be stated clearly in the comments of the
    source code.  Place the comments, in the beginning of your code.

*/


// Windows specific
#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# define WIN32_EXTRA_LEAN
# include <windows.h>
#endif

// OpenGL/GLUT headers
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "math.h"


#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

struct S3dVector
{
    GLfloat x,y,z;
};

struct S2dVector
{
    GLfloat x,y;
};

S3dVector F3dVector ( GLfloat x, GLfloat y, GLfloat z );


class Person{
public:
    Person(){

    }

    ~Person(){

    }

private:
    S3dVector position;

};

class Shelby{
public:
    Shelby(){

    }

    ~Shelby(){

    }

private:

};

class Camera{
public:
    Camera(){

    }

    ~Camera(){

    }

private:
    S3dVector position;
    S3dVector target;

};



GLint window_width = 800;
GLint window_height = 800;


void drawSnowMan() {

    glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);

// Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f,20,20);

// Draw Eyes
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f,10,10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f,10,10);
    glPopMatrix();

// Draw Nose
    glColor3f(1.0, 0.5, 0.5);
    glRotatef(0.0f,1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f,0.5f,10,2);

    glColor3f(1.0f, 1.0f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();

    Camera.Render();

    glPushMatrix();
    drawSnowMan();
    glPopMatrix();

    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();

    // Draw 36 SnowMen
    for(int i = -3; i < 3; i++)
        for(int j=-3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i*10.0,0,j * 10.0);
            drawSnowMan();
            glPopMatrix();
        }

    glutSwapBuffers();
}

void setup(GLsizei w, GLsizei h) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w/ (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    GLfloat LightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
    GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glEnable(GL_LIGHT1);

}

void resize(GLsizei w, GLsizei h) {
    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void onKeyboardPress(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
    }
    glutPostRedisplay();
}

void onSpecialKeyboardPress(int key, int x, int y){
    glutPostRedisplay();
}

int old_y = 0;
int old_x = 0;
bool dragging = false;

/*! GLUT on mouse button click callback */
void onMouseButton(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      dragging = true;
      old_y = y;
      old_x = x;
    } else {
      dragging = false;
      old_y = 0;
      old_x = 0;
    }
  }
  // Scrolling
  if (button == GLUT_WHEEL_UP || button == GLUT_WHEEL_DOWN) {
    if (state == GLUT_UP) return;
    if (button == GLUT_WHEEL_UP) Camera.MoveZ(0.5f);
    if (button == GLUT_WHEEL_DOWN) Camera.MoveZ(-0.5f);
    glutPostRedisplay();
  }
}

/*! GLUT on active (button pressed) mouse motion callback */
void onActiveMouseMotion(int x, int y) {
  if (dragging) {
    Camera.RotateX((old_y - y)/5.0f);
    Camera.RotateY((old_x - x)/5.0f);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
  }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("COMP-5/6400 Assignment 4");

    Camera.Move( F3dVector(0.0, 1.0, 3.0) );
    Camera.MoveX( 1.0 );

    glutDisplayFunc(display);

    glutReshapeFunc(resize);

    glutKeyboardFunc(onKeyboardPress);
    glutSpecialFunc(onSpecialKeyboardPress);

    glutMouseFunc(onMouseButton);
    glutMotionFunc(onActiveMouseMotion);

    glutMainLoop();

    return 0;
}
