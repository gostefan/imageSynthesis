/*! \file GLUTWindow.h
    \brief 
    \author Wojciech Jarosz
    
    Based off of the Glut Master C++ glut wrapper provided at:
        http://www.stetten.com/george/glutmaster/glutmaster.html
        
    Their copyright notice follows:
 
    ----------------------------------------------------------
 
    George Stetten and Korin Crawford
    copyright given to the public domain
*/
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <OGL/GLUTWindow.h>

namespace OGL
{

GLUTWindow::GLUTWindow()
{
}

GLUTWindow::~GLUTWindow()
{
}

void
GLUTWindow::display()
{
    //dummy function
}

void
GLUTWindow::idle()
{
    //dummy function
}

void
GLUTWindow::keyboard(unsigned char, int, int)
{

}

void
GLUTWindow::keyboardUp(unsigned char, int, int)
{

}

void
GLUTWindow::motion(int, int)
{

}

void
GLUTWindow::mouse(int, int, int, int)
{

}

void
GLUTWindow::passiveMotion(int, int)
{

}

void
GLUTWindow::reshape(int, int)
{

}

void
GLUTWindow::special(int, int, int)
{

}

void
GLUTWindow::specialUp(int, int, int)
{

}

void
GLUTWindow::visibility(int)
{

}

void
GLUTWindow::setWindowID(int newWindowID)
{
    m_windowID = newWindowID;
}

int
GLUTWindow::windowID(void)
{
    return m_windowID;
}

} // namespace OGL
