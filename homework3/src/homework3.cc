#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
#include <math.h>
#include <stdlib.h>

GLint windowHeight = 500;
GLint windowWidth = 500;
GLfloat aspect;

class Person
{
    public:
        Person(GLfloat x, GLfloat y, GLfloat z, GLfloat theta)
        {
            this->x_ = x;
            this->y_ = y;
            this->z_ = z;
            this->theta_ = theta;
            this->scale_ = 1.0;
        }

        Person(){}

        void draw()
        {
            glTranslatef(this->x_, this->y_, this->z_);
            glScalef(this->scale_, this->scale_, this->scale_);
            glRotatef(this->theta_, 0, 0, 1);
            glColor3f(1.0, 0.0, 0.0);
            glutSolidCube(3.0);
        }

        void move(GLfloat moveX, GLfloat moveY, GLfloat moveZ)
        {
            this->x_ += moveX;
            this->y_ += moveY;
            this->z_ += moveZ;
        }

        void rotate(int movement)
        {
            this->theta_ += movement;
            if(this->theta_ >= 360)
                this->theta_ -= 360;
            if(this->theta_ < 0)
                this->theta_ += 360;
        }

        void scale(GLfloat scale)
        {
            this->scale_ += scale;
        }

    private:
        GLfloat x_, y_, z_, theta_, scale_;
};



Person *pp = new Person(0,0,0,1);
GLfloat origin[3] = {0.0, 0.0, 0.0};
GLfloat camera[3] = {5.0, 5.0, 5.0};


void myInit()
{

    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    
	glClearColor(0.4,0.4,0.4,1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    gluLookAt(camera[0], camera[1], camera[2],
              origin[0], origin[1], origin[2],
              0.0f, 0.0f, 1.0f);

    pp->draw();
    glPopMatrix();

    glutSwapBuffers();
}

/* Handle the majority of keyboard keys */
void keyboard(unsigned char key, int x, int y)
{
    switch(key){
        /* The minus key will scale down */
        case 45: pp->scale(-0.1); break;
        /* The '=' key will scale up */
        case 61: pp->scale(0.1); break;
        /* ESC exits */
        case 27: exit(EXIT_SUCCESS); break;
    }
    glutPostRedisplay();
}

/* Handle the special keys */
void keyboardSpecial(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT: pp->rotate(5); break;
        case GLUT_KEY_RIGHT: pp->rotate(-5); break;
        case GLUT_KEY_UP: pp->move(0.1, 0, 0); break;
        case GLUT_KEY_DOWN: pp->move(-0.1, 0, 0); break;
    }
    glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height)
{
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)windowWidth / (GLfloat)windowHeight, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("COMP-5/6400 Assignment 3");

	glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutReshapeFunc(reshape);

    myInit();
	glutMainLoop();

    return 0;
}
