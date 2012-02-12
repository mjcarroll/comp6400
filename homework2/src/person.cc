#include <math.h>
#include <stdio.h>

#include "shelby/person.h"

using namespace person;

namespace person {

    Person::Person(GLint x, GLint y, GLint angle)
    {
        this->x_ = x;
        this->y_ = y;
        this->angle_ = angle;
        this->scale_ = 5;
    }
    
    Person::~Person(){}
    
    void Person::draw()
    {
        glPushMatrix();
        // Translate the person by it's coordinates.
        glTranslatef(this->x_, this->y_, 0.0);
        // Rotate the person by it's angle.
        glRotatef(this->angle_, 0, 0, 1);
        // Scale the person appropriately
        glScalef(this->scale_, this->scale_, 0.0);
        
        // Draw an ellipse for the body.
        glColor3f(0.5, 0.9, 0.5);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
        for (int i =0; i <= 360; i+=5)
            glVertex2f(cos(i * M_PI/180), 2*sin(i*M_PI/180));
        glEnd();
        
        // Draw the arms.
        glBegin(GL_QUADS);
        glVertex2f(0,1.75);
        glVertex2f(0,2.0);
        glVertex2f(2,2.0);
        glVertex2f(2,1.75);
        
        glVertex2f(0,-1.75);
        glVertex2f(0,-2.0);
        glVertex2f(2,-2.0);
        glVertex2f(2,-1.75);
        glEnd();
        
        // Draw a circle for the head.
        glColor3f(0.9, 0.9, 0.9);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0,0);
        for (int i = 0; i <= 360; i+=5)
            glVertex2f(cos(i * M_PI/180), sin(i * M_PI/180));
        glEnd();
        
        glPopMatrix();
    }

    
    void Person::move(bool direction)
    {
        int distance;
        
        if(direction)
            distance = 5;
        else
            distance = -5;
        
        this->x_ += distance * cos(this->angle_ * M_PI/180);
        this->y_ += distance * sin(this->angle_ * M_PI/180);
        
        std::printf("   X: %i, Y: %i\n", this->x_, this->y_);
    }
    
    void Person::rotate(bool direction)
    {
        if(direction)
            this->angle_ += 5;
        else
            this->angle_ -= 5;
        if(this->angle_ >=360)
            this->angle_ -= 360;
        if(this->angle_ < 0)
            this->angle_ += 360;
        
        std::printf("   Theta: %i\n", this->angle_);
    }
    
}
