/*! \file GLUTMaster.h
    \brief 
    \author Wojciech Jarosz
	\copyright Based off of the Glut Master C++ glut wrapper provided at:
        http://www.stetten.com/george/glutmaster/glutmaster.html
        
    Their copyright notice follows:
 
    ----------------------------------------------------------
 
    George Stetten and Korin Crawford
    copyright given to the public domain
*/
#ifndef OGLUT_GLUT_MASTER_H
#define OGLUT_GLUT_MASTER_H

namespace OGL
{

class GLUTWindow;

//! Master GLUT controller wrapper class.
/*!
    This class encapsulates the control of GLUT and the main event-loop.
*/
class GLUTMaster
{
private:
    static void display();
    static void idle();
    static void keyboard(unsigned char key, int x, int y);
    static void keyboardUp(unsigned char key, int x, int y);
    static void motion(int x, int y);
    static void mouse(int button, int state, int x, int y);
    static void passiveMotion(int x, int y);
    static void reshape(int w, int h);
    static void special(int key, int x, int y);
    static void specialUp(int key, int x, int y);
    static void visibility(int visible);

    static int  m_currentIdleWindow;
    static bool m_idleFunctionEnabled;
    static bool m_passiveMotionFunctionEnabled;

public:
    GLUTMaster(int* argc, char*argv[]);
    ~GLUTMaster();

    void  createWindow(const char * setTitle, GLUTWindow * glutWindow);
    void  mainLoop();
    void  fullScreen();

    static void  disableIdleFunction();
    static void  enableIdleFunction();
    static bool  idleFunctionEnabled();
    
    static void  disablePassiveMotionFunction();
    static void  enablePassiveMotionFunction();
    static bool  passiveMotionFunctionEnabled();

    int   idleSetToCurrentWindow();
    void  setIdleToCurrentWindow();
};

} // namespace OGL

#endif // OGLUT_GLUT_MASTER_H
