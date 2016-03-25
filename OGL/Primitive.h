/*! \file Primitive.h
    \author Wojciech Jarosz
    \brief This file includes some utility routines to draw common geometric
    primitives.
*/
#ifndef OGL_PRIMITIVE_H
#define OGL_PRIMITIVE_H

namespace OGL
{

namespace Primitive
{
    void drawAxes(float length);
    void drawGrid(float size);
    void drawTextOverlay(const char* str, int windowWidth, int windowHeight);

} // namespace Primtive

} // namespace OGL

#endif
