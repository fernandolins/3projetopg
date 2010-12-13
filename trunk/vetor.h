#ifndef VETOR_H_INCLUDED
#define VETOR_H_INCLUDED

#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>

class vetor {

    public:

    GLdouble x;
    GLdouble y;
    GLdouble z;

    vetor(){};
    vetor(GLdouble x, GLdouble y, GLdouble z);

};

#endif // VETOR_H_INCLUDED
