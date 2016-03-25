/*! \file text.cpp
    \author Wojciech Jarosz
*/
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

//------------------------------------------------------------------------------
// File : glvutext.cpp
//------------------------------------------------------------------------------
// GLVU : Copyright 1997 - 2002
//        The University of North Carolina at Chapel Hill
//------------------------------------------------------------------------------
// Permission to use, copy, modify, distribute and sell this software and its
// documentation for any purpose is hereby granted without fee, provided that
// the above copyright notice appear in all copies and that both that copyright
// notice and this permission notice appear in supporting documentation.
// Binaries may be compiled with this software without any royalties or
// restrictions.
//
// The University of North Carolina at Chapel Hill makes no representations
// about the suitability of this software for any purpose. It is provided
// "as is" without express or implied warranty.

//============================================================================
// glvutext.cpp : bitmap text drawing routine; works like "printf"
//============================================================================

#include <OGL/Core.h>
#include <OGL/Text.h>
#include <stdio.h>
#include <stdarg.h>

//using namespace Util;

namespace OGL
{

namespace
{
//-------------------------------------------------------------------------------
// Storage for the current font needed by the Text() routine. Global state for
// the text module. Access functions are provided. The following fonts are available
// and are defined in glut.h(fontids are on the left):
//  0: GLUT_BITMAP_8_BY_13        : 13 pixels high
//  1: GLUT_BITMAP_9_BY_15        : 15 pixels high
//  2: GLUT_BITMAP_TIMES_ROMAN_10 : 10 pixels high
//  3: GLUT_BITMAP_TIMES_ROMAN_24 : 24 pixels high
//  4: GLUT_BITMAP_HELVETICA_10   : 10 pixels high
//  5: GLUT_BITMAP_HELVETICA_12   : 12 pixels high
//  6: GLUT_BITMAP_HELVETICA_18   : 18 pixels high
//-------------------------------------------------------------------------------
void *_currentFont = GLUT_BITMAP_8_BY_13;

void *fonts[] = { GLUT_BITMAP_8_BY_13,
                  GLUT_BITMAP_9_BY_15,
                  GLUT_BITMAP_TIMES_ROMAN_10,
                  GLUT_BITMAP_TIMES_ROMAN_24,
                  GLUT_BITMAP_HELVETICA_10,
                  GLUT_BITMAP_HELVETICA_12,
                  GLUT_BITMAP_HELVETICA_18 };
int fontHeights[] = { 13, 15, 10, 24, 10, 12, 18 };

char*
makeMessage(const char* format, va_list args)
{
    // Guess we need no more than 100 bytes.
    int n, size = 100;
    char* p;
    
    if ((p = new char[size]) == 0)
        return 0;
    while (1)
    {
        // Try to print in the allocated space.
        n = vsnprintf(p, size, format, args);
    
        // If that worked, return the string.
        if (n > -1 && n < size)
            return p;
    
        // Else try again with more space.
        if (n > -1)     // glibc 2.1
            size = n+1; // precisely what is needed
        else            // glibc 2.0
            size *= 2;  // twice the old size
    
        delete [] p;
        if ((p = new char[size]) == 0)
            return 0;
    }
}

} // namespace

void* currentFont() { return _currentFont; }
void setCurrentFont(void *font) { _currentFont = font; }
void setCurrentFont(int fontid) { _currentFont = fonts[fontid]; }


//-------------------------------------------------------------------------------
// Get the height in pixels for a specified font. Needed for calculating the
// Yoffset to go to the next line. There are several different ways:
// use the current font, specify font by id or explicitly.
//-------------------------------------------------------------------------------
int
fontHeight(void *font)
{
    if (font == GLUT_BITMAP_8_BY_13) return 13;
    else if (font == GLUT_BITMAP_9_BY_15) return 15;
    else if (font == GLUT_BITMAP_TIMES_ROMAN_10) return 10;
    else if (font == GLUT_BITMAP_TIMES_ROMAN_24) return 24;
    else if (font == GLUT_BITMAP_HELVETICA_10) return 10;
    else if (font == GLUT_BITMAP_HELVETICA_12) return 12;
    else if (font == GLUT_BITMAP_HELVETICA_18) return 18;

    return 0;
}


int
fontHeight()
{
    return fontHeight(_currentFont);
}


int
fontHeight(int fontid)
{
    return fontHeights[fontid];
}


//-------------------------------------------------------------------------------
// BITMAP TEXT ROUTINE: draws the printf style format string(with args) with
// the bottom-left corner of the string at pixel position(x,y) in the current
// window with the current color. The bottom-left corner of the window is at
//(0,0). Many different fonts are available(see currentFont above):
// The current raster position is updated to the end of the string; check with:
//  float CurrentRasterPos[4];
//  glGetFloatv(GL_CURRENT_RASTER_POSITION,CurrentRasterPos);
//  x=CurrentRasterPos[0]+0.5;
//  y=CurrentRasterPos[1]+0.5;
// Setting negative values for x or y will avoid the update of the raster pos;
// this is useful for continuing more text on the same line after a previous
// call to Text(). Newline chars('\n') in the string advance the raster position
// to the next line down(Newline chars are ignored in string continuing on same line).
//-------------------------------------------------------------------------------
void
glText(int x, int y, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    char* buffer;
    if (!(buffer = makeMessage(format, args)))
        return;

    va_end(args);

    int WW = glutGet((GLenum)GLUT_WINDOW_WIDTH);
    int WH = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

    glPushAttrib(GL_LIGHTING_BIT);
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        {
            glDisable(GL_DITHER);
            glPushAttrib(GL_DEPTH_BUFFER_BIT);
            {
                glDisable(GL_DEPTH_TEST);
            
                glPushAttrib(GL_VIEWPORT_BIT);
                {
                    glViewport(0, 0, WW, WH);
                    glMatrixMode(GL_PROJECTION);
                    glPushMatrix();
                    {
                        glLoadIdentity();
                        gluOrtho2D(0, WW, 0, WH);
                        glMatrixMode(GL_MODELVIEW);
                        glPushMatrix();
                        {
                            glLoadIdentity();
                        
                            int height = fontHeight(_currentFont);
                        
                            if (x >= 0 && y >= 0)
                                glRasterPos2i(x, y);
                        
                            for (char *p = buffer; *p; p++)
                            {
                                if (*p == '\n')
                                {
                                    if (x>=0 && y>=0)
                                    {
                                        y -= height;
                                        glRasterPos2i(x, y);
                                    }
                                }
                                else
                                {
                                    glutBitmapCharacter(_currentFont, *p);
                                }
                            }
                            
                            delete [] buffer;
                        
                            glMatrixMode(GL_PROJECTION);
                        }
                        glPopMatrix();
                        glMatrixMode(GL_MODELVIEW);
                    }
                    glPopMatrix();
                }
                glPopAttrib();
            }
            glPopAttrib();
        }
        glPopAttrib();
    }
    glPopAttrib();
}

//-------------------------------------------------------------------------------
// BITMAP TEXT ROUTINE: same as Text, but give 3D coordinates in modelspace.
//-------------------------------------------------------------------------------
void
glText3D(float x, float y, float z, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    
    char* buffer;
    if (!(buffer = makeMessage(format, args)))
        return;

    va_end(args);

    glPushAttrib(GL_LIGHTING_BIT);
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        {
            glDisable(GL_DITHER);
            glPushAttrib(GL_DEPTH_BUFFER_BIT);
            {
                glDisable(GL_DEPTH_TEST);
            
                int height = fontHeight(_currentFont);
            
                glRasterPos3f(x, y, z);
            
                for (char *p = buffer; *p; p++)
                {
                    if (*p == '\n')
                    {
                        y -= height;
                        glRasterPos3f(x, y, z);
                    }
                    else
                    {
                        glutBitmapCharacter(_currentFont, *p);
                    }
                }
                delete [] buffer;
            }
            glPopAttrib();
        }
        glPopAttrib();
    }
    glPopAttrib();
}

} // end namespace OGL
