/* COMP6400 Homework 1
   Michael Carroll <carromj@auburn.edu>
 
 Project Requirements:
 1.      The window size should be at least 500x500.
 2.      The window’s title should be “COMP-5/6400 Assignment 1”
 3.      You should use at least four different colors in your scene.
 4.      You should have at least 20 primitives in your scene.
 5.      The design should make common sense.

The assignment is to design and draw a building footprint and the outline of this complex. Assume you are looking down from the sky. Appropriate details, such as the entrance, walkways, and steps/stairs should appear in your design. There are many other things you can do as well (if you wish) like trees, grass, bike-racks, people, etc.  But they are optional. Use your imagination.
*/

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

// OpenGL
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void myinit()
{
    glClearColor(0.1, 0.4, 0.1, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, 600.0f, 0.0f, 600.0f);
    glMatrixMode(GL_MODELVIEW);
}

/**
 * drawOverhang
 *
 * Draw a overhang found on the shelby buildings, used frequently.
 */
void drawOverhang(float x, float y, float angle)
{
    glPushMatrix(); // Save this translation/rotation for later
    glTranslatef(x, y, 0.0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 21.0);
        glVertex2f(21.0, 5.0);
        glVertex2f(21.0, 0.0);
    glEnd();

    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 21.0);
        glVertex2f(-21.0, 5.0);
        glVertex2f(-21.0, 0.0);
    glEnd();
    glPopMatrix(); // Restore translation/rotation
}

/**
 * drawPhaseOne
 *
 * Draw buildings for Shelby phase one (Industrial and Software buildings)
 */
void drawPhaseOne(void)
{
    glBegin(GL_QUADS);
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(5,    0);
        glVertex2f(40,   35);
        glVertex2f(40,  150);
        glVertex2f(5,   115);

        glVertex2f(5, 115);
        glVertex2f(195,115);
        glVertex2f(160,150);
        glVertex2f(40, 150);

        glColor3f(0.42, 0.42, 0.42);
        glVertex2f(75, 0);
        glVertex2f(40, 35);
        glVertex2f(40, 150);
        glVertex2f(75, 115);

        glVertex2f(5, 185);
        glVertex2f(195, 185);
        glVertex2f(160, 150);
        glVertex2f(40, 150);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3f(0.43, 0.43, 0.43);
        glVertex2f(5,0);
        glVertex2f(75,0);
        glVertex2f(40,40);

        glVertex2f(5,115);
        glVertex2f(5,185);
        glVertex2f(40,150);

        glVertex2f(195, 115);
        glVertex2f(195, 185);
        glVertex2f(160,150);
    glEnd();

    drawOverhang(0.0, 21.0, -90);
    drawOverhang(80.0, 21.0, 90);
    drawOverhang(26,190,180);
    drawOverhang(174,190,180);
    drawOverhang(174,110,0);
}

/** 
 * drawPhaseTwo
 *
 * Draw buildings for Shelby phase two (Wiggins Mechanical building and the
 * Advanced Engineering Research Lab)
 *
 * @param garageSize - The two phase two buildings have outcroppings on the
 * Southern side, on Wiggins, this is a garage, and on AERL, it's a clean room.
 */

void drawPhaseTwo(float garageSize)
{
    /* Draw the Wiggins Mechanical Engineering Building */
    glBegin(GL_QUADS);
        /* Two quad roof sections */
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(0.0, 5.0);
        glVertex2f(40.0, 50.0);
        glVertex2f(160.0, 50.0);
        glVertex2f(200.0, 5.0);
    
        glColor3f(0.42f, 0.42f, 0.42f);
        glVertex2f(0.0, 95.0);
        glVertex2f(40.0, 50.0);
        glVertex2f(160.0, 50.0);
        glVertex2f(200.0, 95.0);

        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(100.0-47.5, 5);
        glVertex2f(100.0-47.5, 5-garageSize);
        glVertex2f(100.0+47.5, 5- garageSize);
        glVertex2f(100.0+47.5, 5);
    glEnd();
    glBegin(GL_TRIANGLES);
        /* Two triangle roof sections (end pieces) */
        glColor3f(0.43f, 0.43f, 0.43f);
        glVertex2f(0.0, 5.0);
        glVertex2f(40.0, 50.0);
        glVertex2f(0.0, 95.0);

        glVertex2f(200.0, 5.0);
        glVertex2f(160.0, 50.0);
        glVertex2f(200.0, 95.0);
    glEnd();
    drawOverhang(21.0, 0.0, 0.0);
    drawOverhang(200.0-21.0, 0.0, 0.0);
    drawOverhang(200.0-21.0, 100.0, 180.0);
    drawOverhang(21.0, 100.0, 180.0);
}

void drawCenter(void)
{
    glBegin(GL_QUADS);
        glColor3f(0.45f, 0.45f, 0.45f);
        glVertex2f(0,5);
        glVertex2f(50,5);
        glVertex2f(5,55);
        glVertex2f(0,55);

        glVertex2f(0,100);
        glVertex2f(50,100);
        glVertex2f(5,55);
        glVertex2f(0,55);

        glColor3f(0.35f, 0.35f, 0.35f);
        glVertex2f(0,115);
        glVertex2f(35,115);
        glVertex2f(35,100);
        glVertex2f(0,65);
        
        glVertex2f(40,75);
        glVertex2f(47,68);
        glVertex2f(77,68);
        glVertex2f(84,75);

        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex2f(0,0);
        glVertex2f(35,0);
        glVertex2f(35,5);
        glVertex2f(0,40);

        glVertex2f(50,5);
        glVertex2f(50,100);
        glVertex2f(5,55);
        glVertex2f(5,45);

        glVertex2f(40,75);
        glVertex2f(47,82);
        glVertex2f(77,82);
        glVertex2f(84,75);
    glEnd();
}

void display( void)
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the window
    glLoadIdentity();

    glTranslatef(20.0,0,0);

    // Wiggins Mechanical engineering building
    // 13 primitives
    glPushMatrix();
    glTranslatef(10.0, 100.0, 0.0);
    drawPhaseTwo(35.0); 
    glPopMatrix();

    // Advanced Engineering Research Lab
    // 13 primitives
    glPushMatrix();
    glTranslatef(340.0, 100.0, 0.0);
    drawPhaseTwo(10.0);
    glPopMatrix();

    // Industrial and Systems Engineering/Dean's Suite
    glPushMatrix();
    glTranslatef(10, 265, 0);
    drawPhaseOne();
    glPopMatrix();

    // Computer Science and Software Engineering
    glPushMatrix();
    // Create a matrix to reflect across the y-axis (x = -x)
    float m[] = {-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,  0, 0, 0, 1};
    glTranslatef(540,265,0);
    glMultMatrixf(m);
    drawPhaseOne();
    glPopMatrix();

    // Center Building
    // 14 primitives
    glPushMatrix();
    glTranslatef(275,330,0);
    drawCenter();
    glMultMatrixf(m);
    drawCenter();
    glPopMatrix();

    glFlush(); // Clear buffers
}

void main(int argc, char** argv)
{
    /* Standard GLUT initialization */
    glutInit(&argc, argv);
    /* Set to single buffer, with RGB color space */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    /* 600x600 per the project requirements */
    glutInitWindowSize(600, 600);
    /* Place window on the display */
    glutInitWindowPosition(0,0);
    /* Create the window now */
    glutCreateWindow("COMP-5/6400 Assignment 1");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
}
