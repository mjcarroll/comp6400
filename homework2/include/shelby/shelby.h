#ifndef SHELBY_H
#define SHELBY_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace shelby {

    class ShelbyCenter
    {
    public:
        ShelbyCenter();
        virtual ~ShelbyCenter();
        
        void draw();
        
    private:
        void drawOverhang(GLfloat x, GLfloat y, GLfloat angle);
        void drawPhaseOne(void);
        void drawPhaseTwo(GLfloat garageSize);
        
        void drawCenter(void);
        static void drawLamp();
        static void drawRoad(GLfloat width, GLfloat startColor, GLfloat endColor);
        static void drawStreetLine();
        
        void drawArray(void (*drawFunc)(void),
                       GLfloat x_origin,
                       GLfloat y_origin,
                       GLfloat x,
                       GLfloat y,
                       unsigned x_dim,
                       unsigned y_dim);
    };
}

#endif
