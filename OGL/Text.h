/*! \file Text.h
    \author Wojciech Jarosz
    \brief This file includes some utility routines to draw 2D text on the screen.
	The implementation depends on the \c glutBitmapCharacter() routine and
    GLUT fonts.
 
	\copyright
	GLVU : Copyright 1997 - 2002 The University of North Carolina at Chapel Hill
 
	Permission to use, copy, modify, distribute and sell this software and its 
	documentation for any purpose is hereby granted without fee, provided that 
	the above copyright notice appear in all copies and that both that copyright 
	notice and this permission notice appear in supporting documentation. 
	Binaries may be compiled with this software without any royalties or 
	restrictions. 

	The University of North Carolina at Chapel Hill makes no representations 
	about the suitability of this software for any purpose. It is provided 
	"as is" without express or implied warranty.
*/
#ifndef OGLUT_TEXT_H
#define OGLUT_TEXT_H


namespace OGL
{

/*! 
    Return a pointer to the current font to be used in calls
    to glText() and glText3D().
*/
void * currentFont();

/*! 
    Set the current font. Use one of the GLUT font names.
    Possible values include:
    - GLUT_BITMAP_8_BY_13 
    - GLUT_BITMAP_9_BY_15
    - GLUT_BITMAP_TIMES_ROMAN_10 
    - GLUT_BITMAP_TIMES_ROMAN_24
    - GLUT_BITMAP_HELVETICA_10 
    - GLUT_BITMAP_HELVETICA_12
    - GLUT_BITMAP_HELVETICA_18
    \see setCurrentFont(int)
*/
void setCurrentFont(void *font);

/*! 
    Set the current font. Use a number between 0 and 6, inclusive.
    \see setCurrentFont(void*)
*/
void setCurrentFont(int fontid);


/*!
    Get the height in pixels for a specified font. Needed for calculating the
    Yoffset to go to the next line. 
    \see fontHeight(), fontHeight(int)
*/
int fontHeight(void *font);


/*!
    Get the height in pixels of the current font.
    \see currentFont, fontHeight(void*)
*/
int fontHeight();


/*!
    Get the height in pixels of the current font.
    \see currentFont, fontHeight(), fontHeight(void*)
*/
int fontHeight(int fontid);


/*!
    Draw text at the specified 2D location in screen coordinates in the current
    font.  The text will be drawn in the current OpenGL color.
    The function accepts printf style format specification.
    
    \see setFont, glText3D
*/
void glText(int x, int y, const char *format, ...);


/*!
    Draw text at the specified 3D raster location.
    The text will be drawn in the current OpenGL color.
    The function accepts printf style format specification.
    
    \see setFont, glText
*/
void glText3D(float x, float y, float z, const char *format, ...);

} // namespace OGL

#endif
