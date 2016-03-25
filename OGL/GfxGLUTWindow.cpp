/*! \file GfxGLUTWindow.cpp
    \author Wojciech Jarosz
*/
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <OGL/GfxGLUTWindow.h>
#include <OGL/Core.h>
#include <OGL/Text.h>
#include <OGL/Primitive.h>
#include <stdlib.h>

namespace OGL
{

GfxGLUTWindow::GfxGLUTWindow() :
    m_mouseX(0), m_mouseY(0),
    m_windowWidth(0),
    m_windowHeight(0),
    m_showHelp(false),
    m_showHUD(true)
{

}


GfxGLUTWindow::GfxGLUTWindow(unsigned int mode, int width, int height) :
    m_mouseX(0), m_mouseY(0),
    m_mousePressX(0), m_mousePressY(0),
    m_windowWidth(width),
    m_windowHeight(height),
    m_showHelp(false),
    m_showHUD(true)
{
    glutInitDisplayMode(mode);
    glutInitWindowSize(width, height);
}


GfxGLUTWindow::GfxGLUTWindow(unsigned int mode, int width, int height,
                     int initPositionX, int initPositionY) :
    m_mouseX(0), m_mouseY(0),
    m_windowWidth(width),
    m_windowHeight(height),
    m_showHelp(false),
    m_showHUD(true)
{
    glutInitDisplayMode(mode);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(initPositionX, initPositionY);
}


GfxGLUTWindow::~GfxGLUTWindow()
{
    // empty
}


void
GfxGLUTWindow::quit()
{
    exit(0);
}


void
GfxGLUTWindow::reshape(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;

    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glMatrixMode(GL_MODELVIEW);
}


void
GfxGLUTWindow::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':
        case 'Q':
        case 27:
            quit();
            break;
        case 'h':
            m_showHelp = !m_showHelp;
            break;
        case 'H':
            m_showHUD = !m_showHUD;
            break;
        default:
            return;
    }
}


void
GfxGLUTWindow::mouse(int button, int state, int x, int y)
{
	m_mouseButton = button;
	m_mouseState = state;
    m_mouseX = x;
    m_mouseY = y;
	if (state == GLUT_DOWN)
	{
		m_mousePressX = x;
		m_mousePressY = y;
	}
}


void
GfxGLUTWindow::drawHelp(const char* string) const
{
    if (!m_showHelp)
        return;
        
    Primitive::drawTextOverlay(string, m_windowWidth, m_windowHeight);
}


void
GfxGLUTWindow::drawHUD(const char * string) const
{
    if (!m_showHUD)
        return;
    
    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
        
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, m_windowWidth, 0, m_windowHeight);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();

            glColor3f(0, 0, 0);
            glText(11, m_windowHeight - 16, string);
            glColor3f(1, 1, 1);
            glText(10, m_windowHeight - 15, string);
        glPopMatrix();
    glPopMatrix();

    glPopAttrib(); // GL_TRANSFORM_BIT
    glPopAttrib(); // GL_DEPTH_BUFFER_BIT
}

} // namespace OGL
