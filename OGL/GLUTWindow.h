/*! \file GLUTWindow.h
    \brief 
    \author Wojciech Jarosz
    \copyright Based off of the Glut Master C++ glut wrapper provided at:
        http://www.stetten.com/george/glutmaster/glutmaster.html
        
    Their copyright notice follows:

    ----------------------------------------------------------

    George Stetten and Korin Crawford
    copyright given to the public domain
*/
#ifndef OGLUT_GLUT_WINDOW_H
#define OGLUT_GLUT_WINDOW_H

namespace OGL
{

//! Abstraction of a GLUT GUI window.
/*!
    This class encapsulates the control of a GLUT window.
    GLUTWindow's can be added to the GLUTMaster class which
    manages them.
*/
class GLUTWindow
{
public:
   GLUTWindow();
   virtual ~GLUTWindow();

   virtual void display();
   virtual void idle();
   virtual void keyboard(unsigned char key, int x, int y);
   virtual void keyboardUp(unsigned char key, int x, int y);
   virtual void motion(int x, int y);
   virtual void mouse(int button, int state, int x, int y);
   virtual void passiveMotion(int x, int y);
   virtual void reshape(int w, int h);   
   virtual void special(int key, int x, int y);
   virtual void specialUp(int key, int x, int y);
   virtual void visibility(int visible);

   void    setWindowID(int newWindowID);
   int     windowID(void);

protected:
   int m_windowID;
};

} // namespace OGL

#endif // OGLUT_GLUT_WINDOW_H
