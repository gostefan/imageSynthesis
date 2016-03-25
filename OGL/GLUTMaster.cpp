/*! \file GLUTMaster.cpp
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

#include <OGL/GLUTMaster.h>
#include <OGL/GLUTWindow.h>
#include <OGL/Core.h>
#include <iostream>

namespace OGL
{

namespace
{

#define MAX_NUMBER_OF_WINDOWS 256
GLUTWindow * viewPorts[MAX_NUMBER_OF_WINDOWS];

} // namespace

int  GLUTMaster::m_currentIdleWindow   = 0;
bool GLUTMaster::m_idleFunctionEnabled = false;
bool GLUTMaster::m_passiveMotionFunctionEnabled = false;

GLUTMaster::GLUTMaster(int* argc, char*argv[])
{
    // Initialize GLUT
    glutInit(argc, argv);
}

GLUTMaster::~GLUTMaster()
{
}

void
GLUTMaster::display()
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->display();
}

void
GLUTMaster::idle()
{
    if (m_idleFunctionEnabled && m_currentIdleWindow)
    {
        glutSetWindow(m_currentIdleWindow);
        viewPorts[m_currentIdleWindow]->idle();
    }
}

void
GLUTMaster::keyboard(unsigned char key, int x, int y)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->keyboard(key, x, y);
}

void
GLUTMaster::keyboardUp(unsigned char key, int x, int y)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->keyboardUp(key, x, y);
}

void
GLUTMaster::motion(int x, int y)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->motion(x, y);
}

void
GLUTMaster::mouse(int button, int state, int x, int y)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->mouse(button, state, x, y);
}

void
GLUTMaster::passiveMotion(int x, int y)
{
    if (m_passiveMotionFunctionEnabled)
    {
        int windowID = glutGetWindow();
        viewPorts[windowID]->passiveMotion(x, y);
    }
}

void
GLUTMaster::reshape(int w, int h)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->reshape(w, h);
}

void
GLUTMaster::special(int key, int x, int y)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->special(key, x, y);
}

void
GLUTMaster::specialUp(int key, int x, int y)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->specialUp(key, x, y);
}

void
GLUTMaster::visibility(int visible)
{
    int windowID = glutGetWindow();
    viewPorts[windowID]->visibility(visible);
}

void
GLUTMaster::createWindow(const char * setTitle, GLUTWindow * glutWindow)
{
    // Open new window, record its windowID ,
    int windowID = glutCreateWindow(setTitle);
    glutWindow->setWindowID(windowID);

    // Store the address of new window in global array
    // so GLUTMaster can send events to propoer callback functions.
    viewPorts[windowID] = glutWindow;

#ifdef WIN32
	// Initialize Glew on Windows
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}
#endif

    // Hand address of universal static callback functions to Glut.
    // This must be for each new window, even though the address are constant.

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutReshapeFunc(reshape);
    glutVisibilityFunc(visibility);
}

void
GLUTMaster::mainLoop()
{
    glutMainLoop();
}

void
GLUTMaster::fullScreen()
{
    glutFullScreen();
}

void
GLUTMaster::disableIdleFunction()
{
    m_idleFunctionEnabled = false;
    glutIdleFunc(0);
}

void
GLUTMaster::enableIdleFunction()
{
    m_idleFunctionEnabled = true;
    glutIdleFunc(idle);
}

bool
GLUTMaster::idleFunctionEnabled()
{
    // Is passive motion function enabled?
    return m_idleFunctionEnabled;
}

void
GLUTMaster::disablePassiveMotionFunction()
{
    m_passiveMotionFunctionEnabled = false;
    glutPassiveMotionFunc(0);
}

void
GLUTMaster::enablePassiveMotionFunction()
{
    m_passiveMotionFunctionEnabled = true;
    glutPassiveMotionFunc(passiveMotion);
}

bool
GLUTMaster::passiveMotionFunctionEnabled()
{
    // Is passive motion function enabled?
    return m_passiveMotionFunctionEnabled;
}

int
GLUTMaster::idleSetToCurrentWindow()
{
    // Is current idle window same as current window?
    return m_currentIdleWindow == glutGetWindow();
}

void
GLUTMaster::setIdleToCurrentWindow()
{
    m_currentIdleWindow = glutGetWindow();
}

} // namespace OGL

