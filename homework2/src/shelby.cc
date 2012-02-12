#include "shelby/shelby.h"
#include <math.h>

using namespace shelby;

namespace shelby {
  
    ShelbyCenter::ShelbyCenter(){
    
    }
    
    ShelbyCenter::~ShelbyCenter(){
    
    }
    
    void ShelbyCenter::draw(){
        glPushMatrix();
        glTranslatef(10.0, 100.0, 0.0);
        this->drawPhaseTwo(35.0); 
        glPopMatrix();
        
        // Advanced Engineering Research Lab
        // 13 primitives
        glPushMatrix();
        glTranslatef(340.0, 100.0, 0.0);
        this->drawPhaseTwo(10.0);
        glPopMatrix();
        
        // Industrial and Systems Engineering/Dean's Suite
        glPushMatrix();
        glTranslatef(10, 265, 0);
        this->drawPhaseOne();
        glPopMatrix();
        
        // Computer Science and Software Engineering
        glPushMatrix();
        // Create a matrix to reflect across the y-axis (x = -x)
        float m[] = {-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,  0, 0, 0, 1};
        glTranslatef(540,265,0);
        glMultMatrixf(m);
        this->drawPhaseOne();
        glPopMatrix();
        
        // Center Building
        // 14 primitives
        glPushMatrix();
        glTranslatef(275,330,0);
        this->drawCenter();
        glMultMatrixf(m);
        this->drawCenter();
        glPopMatrix();
        
        this->drawArray(&ShelbyCenter::drawLamp, 250, 5, 100, 200, 2, 4); 
        
        glLoadIdentity();
        glTranslatef(0,500,0);
        
        ShelbyCenter::drawRoad(30, 0.0, 0.3);
        
        glLoadIdentity();
        this->drawArray(&ShelbyCenter::drawStreetLine, 
                        0, 512, 600, 1, 15, 1);
        
        glLoadIdentity();
        glTranslatef(0,490,0);
        ShelbyCenter::drawRoad(7, 0.4, 0.5);
        
        glTranslatef(0,43,0);
        ShelbyCenter::drawRoad(7, 0.4, 0.5);
        
    }
    
    void ShelbyCenter::drawPhaseOne(){
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
        
        this->drawOverhang(0.0, 21.0, -90);
        this->drawOverhang(80.0, 21.0, 90);
        this->drawOverhang(26,190,180);
        this->drawOverhang(174,190,180);
        this->drawOverhang(174,110,0);
    }
    
    void ShelbyCenter::drawPhaseTwo(GLfloat garageSize){
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
        this->drawOverhang(21.0, 0.0, 0.0);
        this->drawOverhang(200.0-21.0, 0.0, 0.0);
        this->drawOverhang(200.0-21.0, 100.0, 180.0);
        this->drawOverhang(21.0, 100.0, 180.0);
    }
    
    void ShelbyCenter::drawCenter(){
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
    
    void ShelbyCenter::drawOverhang(GLfloat x, GLfloat y, GLfloat angle){
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
    
    void ShelbyCenter::drawLamp(){
        glShadeModel(GL_SMOOTH);
        glPushMatrix();
        glScalef(2,2,0);
        
        glColor3f(1.0,0.9,0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
        for (int i = 0; i <= 360; i+=5)
        {
            glColor3f(0.1, 0.4, 0.1);
            glVertex2f(4*cos(i * M_PI/180), 4*sin(i*M_PI/180));
        }
        glEnd();

        
        glColor3f(0.1, 0.1, 0.1);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
        for (int i = 0; i <= 360; i+=5)
            glVertex2f(cos(i * M_PI/180), sin(i * M_PI/180));
        glEnd();
        

        glPopMatrix();
    }
    
    void ShelbyCenter::drawArray(void (*drawFunc)(void),
                                 GLfloat x_origin,
                                 GLfloat y_origin,
                                 GLfloat x,
                                 GLfloat y,
                                 unsigned x_dim,
                                 unsigned y_dim)
    {
        GLfloat x_stride = x/x_dim;
        GLfloat y_stride = y/y_dim;
        
        for (int i = 0; i < x_dim; i++)
        {
            glPushMatrix();
            glTranslatef(x_origin + i*x_stride, y_origin, 0);
            for (int j = 0; j < y_dim; j++)
            {
                glTranslatef(0, y_stride, 0);
                (*drawFunc)();
            }
            glPopMatrix();
        }
    }
    
    void ShelbyCenter::drawRoad(GLfloat width, GLfloat startColor, GLfloat endColor)
    {
        glShadeModel(GL_SMOOTH);
        glPushMatrix();
        
        glBegin(GL_QUADS);
        glColor3f(startColor, startColor, startColor);
        glVertex2f(0,0);
        glVertex2f(600,0);
        glColor3f(endColor, endColor, endColor);
        glVertex2f(600,width);
        glVertex2f(0,width);
        glEnd();
        
        glPopMatrix();
    }
    
    void ShelbyCenter::drawStreetLine()
    {
        glPushMatrix();
        
        glBegin(GL_QUADS);
        glColor3f(0.9, 0.9, 1.0);
        glVertex2f(0,0);
        glVertex2f(10,0);
        glColor3f(0.7, 0.7, 0.0);
        glVertex2f(10,5);
        glVertex2f(0,5);
        glEnd();
        
        glPopMatrix();
        
    }
}
