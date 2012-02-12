#ifndef PERSON_H
#define PERSON_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

namespace person {

    class Person
    {
    public:
        Person(GLint x = 0, GLint y = 0, GLint angle = 0);
        virtual ~Person();
        
        void draw();
        void move(bool direction);
        void rotate(bool direction);
        
    private:
        GLint x_, y_, angle_;
        GLfloat scale_;
    };
        
}

#endif
