/*! \file Core.h
    \brief Just includes all GL headers
    \author Wojciech Jarosz
*/
#ifndef OGL_CORE_H
#define OGL_CORE_H

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#endif // OGL_H
