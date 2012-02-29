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

#include <stdio.h>

#include "shelby/shelby.h"
#include "shelby/person.h"

using namespace shelby;
using namespace person;

ShelbyCenter *sc = new ShelbyCenter;
Person *pp = new Person(300,200,90);

float windowSize = 600.0;

void myInit()
{
	glClearColor(0.1, 0.4, 0.1, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, windowSize, 0.0f, windowSize);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(20.0, 0, 0);
    sc->draw();

    glLoadIdentity();
    pp->draw();

    glutSwapBuffers();
}

/* Handle the majority of keyboard keys */
void keyboard(unsigned char key, int x, int y)
{
    switch(key){
        /* The minus key will scale down */
        case 45: pp->scale(false); break;
        /* The '=' key will scale up */
        case 61: pp->scale(true); break;
        /* ESC exits */
        case 27: exit(0); break;
        default: printf("   Keyboard %c == %d\n", key, key); break;
    }
    glutPostRedisplay();
}

/* Handle the special keys */
void keyboardSpecial(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_LEFT: pp->rotate(true); break;
        case GLUT_KEY_RIGHT: pp->rotate(false); break;
        case GLUT_KEY_UP: pp->move(true); break;
        case GLUT_KEY_DOWN: pp->move(false); break;
        default:
            printf("   Special key %c == %d\n", key, key);
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowSize, windowSize);
	glutInitWindowPosition(0,0);

	glutCreateWindow("COMP-5/6400 Assignment 2");

	glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

    myInit();
	glutMainLoop();

    return 0;
}
