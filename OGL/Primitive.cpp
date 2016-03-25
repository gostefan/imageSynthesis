/*! \file Primitive.cpp
    \author Wojciech Jarosz
*/
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <OGL/Core.h>
#include <OGL/Primitive.h>
#include <OGL/Text.h>
#include <Math/MathGL.h>
#include <Math/FastMath.h>
#include <stdlib.h>

using namespace Math;

namespace OGL
{

void
Primitive::drawAxes(float length)
{
    // the stroke characters 'X' 'Y' 'Z'
    static float xx[]   = { 0.0f, 1.0f, 0.0f, 1.0f};
    static float xy[]   = {-0.5f, 0.5f, 0.5f, -0.5f};
    static int xorder[] = {1, 2, -3, 4};
    static float yx[]   = {0.0f, 0.0f, -0.5f, 0.5f};
    static float yy[]   = {0.0f, 0.6f, 1.0f, 1.0f};
    static int yorder[] = {1, 2, 3, -2, 4};
    static float zx[]   = {1.0f, 0.0f, 1.0f, 0.0f, 0.25f, 0.75f};
    static float zy[]   = {0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f};
    static int zorder[] = {1, 2, 3, 4, -5, 6};

    int i, j;       // counters

    glPushAttrib(GL_CURRENT_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(length, 0.0f, 0.0f);
    glEnd();

    float fact = 0.10f * length;     // character scale factor
    float base = 1.10f * length;     // character start location

    glBegin(GL_LINE_STRIP);
    for (i = 0; i < 4; i++)
    {
        j = xorder[i];
        if (j < 0)
        {
            glEnd();
            glBegin(GL_LINE_STRIP);
            j = -j;
        }
        j--;
        glVertex3f(base + fact*xx[j], fact*xy[j], 0.0f);
    }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, length, 0.0f);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (i = 0; i < 5; i++)
    {
        j = yorder[i];
        if (j < 0)
        {
            glEnd();
            glBegin(GL_LINE_STRIP);
            j = -j;
        }
        j--;
        glVertex3f(fact*yx[j], base + fact*yy[j], 0.0f);
    }
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, length);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (i = 0; i < 6; i++)
    {
        j = zorder[i];
        if (j < 0)
        {
            glEnd();
            glBegin(GL_LINE_STRIP);
            j = -j;
        }
        j--;
        glVertex3f(0.0f, fact*zy[j], base + fact*zx[j]);
    }
    glEnd();
    
    glPopAttrib(); // GL_CURRENT_BIT
}


void
Primitive::drawGrid(float size)
{
    glPushAttrib(GL_CURRENT_BIT);
    
    // draw the course black grid
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex3f(-size, 0.0f,   0.0f);
        glVertex3f( size, 0.0f,   0.0f);
        glVertex3f(  0.0f, 0.0f, -size);
        glVertex3f(  0.0f, 0.0f,  size);
    glEnd();

    // draw the fine grey grid
    float j = powf(2.f, (float) ceil2Int(log2f(size)))/16.0f;
    int ii = 0;
    float i = 0;
    for (; i <= size; i += j, ii++)
    {
        if (ii % 4 == 0)
            glColor3f(0.25f, 0.25f, 0.25f);
        else
            glColor3f(0.40f, 0.40f, 0.40f);

        glBegin(GL_LINES);
            glVertex3f( 0.0f, 0.0f,     i);
            glVertex3f( size, 0.0f,     i);
            glVertex3f(    i, 0.0f,  0.0f);
            glVertex3f(    i, 0.0f,  size);
    
            glVertex3f( 0.0f, 0.0f,     i);
            glVertex3f(-size, 0.0f,     i);
            glVertex3f(    i, 0.0f,  0.0f);
            glVertex3f(    i, 0.0f, -size);
    
            glVertex3f( 0.0f, 0.0f,    -i);
            glVertex3f( size, 0.0f,    -i);
            glVertex3f(   -i, 0.0f,  0.0f);
            glVertex3f(   -i, 0.0f,  size);
    
            glVertex3f( 0.0f, 0.0f,    -i);
            glVertex3f(-size, 0.0f,    -i);
            glVertex3f(   -i, 0.0f,  0.0f);
            glVertex3f(   -i, 0.0f, -size);
        glEnd();
    }
    
    glPopAttrib(); // GL_CURRENT_BIT
}


void
Primitive::drawTextOverlay(const char* str, int windowWidth, int windowHeight)
{
	int top = 50;
	
	glPushAttrib(GL_ENABLE_BIT);
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_POLYGON_BIT);
	glPushAttrib(GL_TRANSFORM_BIT);
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_PROJECTION);
	
	// Now set scissor to smaller red sub region
	glScissor(top-1, top-1, windowWidth-2*(top-1), windowHeight-2*(top-1));
	glEnable(GL_SCISSOR_TEST);
	
	glPushMatrix();
	{
		glLoadIdentity();
		gluOrtho2D(0, windowWidth, windowHeight, 0);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glLoadIdentity();
			
			glColor4f(0.25f, 0.25f, 0.25f, 0.85f);
			glBegin(GL_QUADS);
			{
				glVertex2i(top, top);
				glVertex2i(top, windowHeight-top);
				glVertex2i(windowWidth-top, windowHeight-top);
				glVertex2i(windowWidth-top, top);
			}
			glEnd();
			
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
			{
				glVertex2i(top, top);
				glVertex2i(top, windowHeight-top);
				glVertex2i(windowWidth-top, windowHeight-top);
				glVertex2i(windowWidth-top, top);
			}
			glEnd();
			
			glColor3f(1.0f, 1.0f, 1.0f);
			glText(top + 10, windowHeight - top -15, str);
		}
		glPopMatrix();
	}
	glPopMatrix();
	
	glDisable(GL_SCISSOR_TEST);

	glPopAttrib(); // GL_COLOR_BUFFER_BIT
	glPopAttrib(); // GL_TRANSFORM_BIT
	glPopAttrib(); // GL_POLYGON_BIT
	glPopAttrib(); // GL_DEPTH_BUFFER_BIT
	glPopAttrib(); // GL_ENABLE_BIT
}


} // end namespace OGL
