/**
 * Michael Carroll <carromj@auburn.edu>
 *
 * COMP6400 Homework #3
 *
 *
 *  Draws a rudimentary engineering student using shaded polygons.
 *      Uses 6 polygons for body
 *      Uses 12 polygons for arms
 *      Uses 12 polygons for legs
 *      Uses 100 polygons for head
 *      Uses 6 polygons for backpack
 *
 *  Also utilizies orthogonal projection and GL lighting.
 *
 *  Use Up and Down to move forwards and backwards, left and right to rotate,
 *  q to quit.  
 *
 *  Window resizing works.
 */

// Defines for Windows
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

// Defines for MAC OSX and Linux
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// Stdlib includes
#include <math.h>
#include <stdlib.h>

// A few helpful defines.
#define SQRT2  1.414
#ifndef M_PI
    #define M_PI 3.14159
#endif

using namespace std;

GLint windowHeight = 500;
GLint windowWidth = 500;
GLfloat aspect;

class Person
{
    public:
    /**
     * Constructor for the Engineering Student
     *
     * @param x
     *      initial x position
     * @param y
     *      initial y position
     * @param z
     *      initial z position
     * @param theta
     *      initial angle
     */
        Person(GLfloat x, GLfloat y, GLfloat z, GLfloat theta)
        {
            this->x_ = x;
            this->y_ = y;
            this->z_ = z;
            this->theta_ = theta+ M_PI/2;
            this->scale_ = 1.0;
        }

        /**
         * Destructor
         */
        Person(){}

        /**
         * Function called to actually draw the person
         */
        void draw()
        {
            // Translate, Rotate, and Scale by the specified amounts
            glTranslatef(this->x_, this->y_, this->z_);
            glScalef(this->scale_, this->scale_, this->scale_);
            glRotatef(this->theta_, 0, 0, 1);
        
            // Draw left leg
            glPushMatrix();
            glTranslatef(0.0, 0.5, 0.0);
            glScalef(1.0, 0.4, 1.0);
            this->draw_cube(0.4, 0.2, 0.02);
            glPopMatrix();

            // Draw right leg
            glPushMatrix();
            glTranslatef(0.0, -0.5, 0.0);
            glScalef(1.0, 0.4, 1.0);
            this->draw_cube(0.4, 0.2, 0.02);
            glPopMatrix();

            // Draw left arm
            glPushMatrix();
            glTranslatef(0.0, -1.2, 2.0);
            glScalef(0.2, 0.2, 1.0);
            this->draw_cube(0.9, 0.9, 0.9);
            glPopMatrix();

            // Draw torso
            glPushMatrix();
            glTranslatef(0.0, 0.0, 2.0);
            this->draw_cube(0.9, 0.4, 0.02);
            glPopMatrix();

            // Draw right arm
            glPushMatrix();
            glTranslatef(0.0, 1.2, 2.0);
            glScalef(0.2, 0.2, 1.0);
            this->draw_cube(0.9, 0.9, 0.9);
            glPopMatrix();

            // Draw backpack
            glPushMatrix();
            glTranslatef(1.2, 0.0, 2.0);
            glScalef(0.1, 0.4, 0.8);
            this->draw_cube(0.6, 0.4, 0.2);
            glPopMatrix();

            // Head
            glPushMatrix();
            glTranslatef(0.0, 0.0, 4.0);
            this->draw_head();
            glPopMatrix();
        }

        /**
         * Called to move student
         */
        void move(GLfloat moveX, GLfloat moveY, GLfloat moveZ)
        {
            this->x_ += moveX * cos(this->theta_ * M_PI/180);
            this->y_ += moveX * sin(this->theta_ * M_PI/180);
            this->z_ += moveZ;
        }

        /**
         * Called to rotate student
         */
        void rotate(int movement)
        {
            this->theta_ += movement;
            if(this->theta_ >= 360)
                this->theta_ -= 360;
            if(this->theta_ < 0)
                this->theta_ += 360;
        }

        /**
         * Called to scale student
         */
        void scale(GLfloat scale)
        {
            this->scale_ += scale;
        }

        /**
         * Helper funtion to draw the head
         */
        void draw_head(void)
        {
            int i, j;
            int lats = 10;
            int longs = 10;
            double r = 0.2;
            for(i = 0; i <= lats; i++) {
                double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
                double z0  = sin(lat0);
                double zr0 =  cos(lat0);

                double lat1 = M_PI * (-0.5 + (double) i / lats);
                double z1 = sin(lat1);
                double zr1 = cos(lat1);

                glShadeModel(GL_SMOOTH);
                glBegin(GL_QUAD_STRIP);
                for(j = 0; j <= longs; j++) {
                    double lng = 2 * M_PI * (double) (j - 1) / longs;
                    double x = cos(lng);
                    double y = sin(lng);
                    glColor3f(1.0, 0.8, 0.6);
                    glNormal3f(x * zr0, y * zr0, z0);
                    glVertex3f(x * zr0, y * zr0, z0);
                    glNormal3f(x * zr1, y * zr1, z1);
                    glVertex3f(x * zr1, y * zr1, z1);
                }
                glEnd();
            }
        }

        /**
         * Helper function to draw a cube
         */
        void draw_cube(GLfloat baseR, GLfloat baseG, GLfloat baseB)
        {
            glShadeModel(GL_SMOOTH);

            glBegin(GL_QUADS);
                glColor3f(baseR, baseG, baseB);
                glVertex3f(1,1,1);
                glColor3f(baseR-0.01, baseG+0.01, baseB-0.01);
                glVertex3f(-1,1,1);
                glColor3f(baseR+0.01, baseG+0.01, baseB+0.01);
                glVertex3f(-1,-1,1);
                glColor3f(baseR-0.01, baseG-0.02, baseB-0.01);
                glVertex3f(1,-1,1);

                glColor3f(baseR, baseG, baseB);
                glVertex3f(1,1,1);
                glColor3f(baseR-0.01, baseG+0.01, baseB-0.01);
                glVertex3f(1,-1,1);
                glColor3f(baseR+0.01, baseG+0.01, baseB+0.01);
                glVertex3f(1,-1,-1);
                glColor3f(baseR-0.01, baseG-0.02, baseB-0.01);
                glVertex3f(1,1,-1);

                glColor3f(baseR, baseG, baseB);
                glVertex3f(1,1,1);
                glColor3f(baseR-0.01, baseG+0.01, baseB-0.01);
                glVertex3f(1,1,-1);
                glColor3f(baseR+0.01, baseG+0.01, baseB+0.01);
                glVertex3f(-1,1,-1);
                glColor3f(baseR-0.01, baseG-0.02, baseB-0.01);
                glVertex3f(-1,1,1);

                glColor3f(baseR, baseG, baseB);
                glVertex3f(-1,-1,1);
                glColor3f(baseR-0.01, baseG+0.01, baseB-0.01);
                glVertex3f(-1,-1,-1);
                glColor3f(baseR+0.01, baseG+0.01, baseB+0.01);
                glVertex3f(1,-1,-1);
                glColor3f(baseR-0.01, baseG-0.02, baseB-0.01);
                glVertex3f(1,-1,1);

                glColor3f(baseR, baseG, baseB);
                glVertex3f(-1,1,1);
                glColor3f(baseR-0.01, baseG+0.01, baseB-0.01);
                glVertex3f(-1,1,-1);
                glColor3f(baseR+0.01, baseG+0.01, baseB+0.01);
                glVertex3f(-1,-1,-1);
                glColor3f(baseR-0.01, baseG-0.02, baseB-0.01);
                glVertex3f(-1,-1,1);

                glColor3f(baseR, baseG, baseB);
                glVertex3f(-1,1,-1);
                glColor3f(baseR-0.01, baseG+0.01, baseB-0.01);
                glVertex3f(1,1,-1);
                glColor3f(baseR+0.01, baseG+0.01, baseB+0.01);
                glVertex3f(1,-1,-1);
                glColor3f(baseR-0.01, baseG-0.02, baseB-0.01);
                glVertex3f(-1,-1,-1);
            glEnd();
        }
    private:
        GLfloat x_, y_, z_, theta_, scale_;
};

Person *pp = new Person(0,0,0,1);
GLfloat origin[3] = {0.0, 0.0, 0.0};
GLfloat camera[3] = {10.0, 10.0, 10.0};


// Initialization and setup.
void myInit()
{
    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glDepthFunc(GL_LESS);
    
	glClearColor(0.4,0.4,0.4,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set up projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // Set up lighting
    GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
    GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

    glEnable(GL_LIGHTING);              // Set up ambient light.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

    glEnable(GL_LIGHT0);                // Set up sunlight.
    glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

    glEnable(GL_COLOR_MATERIAL);        // Configure glColor().
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

}

// Callback function to update display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // set camera
    glPushMatrix();
    gluLookAt(camera[0], camera[1], camera[2],
              origin[0], origin[1], origin[2],
              0.0f, 0.0f, 1.0f);

    // Draw the person
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

/* Callback for resizing the display window */
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

/* main function */
int main(int argc, char *argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
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
