/*! \file GfxGLUTWindow.h
    \author Wojciech Jarosz
*/
#ifndef OGLUT_DEMO_WINDOW_H
#define OGLUT_DEMO_WINDOW_H

#include <OGL/GLUTMaster.h>
#include <OGL/GLUTWindow.h>

namespace OGL
{

class GfxGLUTWindow : public GLUTWindow
{
public:
    GfxGLUTWindow();
    GfxGLUTWindow(unsigned int mode, int setWidth, int setHeight);
    GfxGLUTWindow(unsigned int mode, int setWidth, int setHeight,
              int setInitPositionX, int setInitPositionY);
    virtual ~GfxGLUTWindow();

    // Event handlers
    virtual void quit();
    virtual void reshape(int x, int y);
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void mouse(int btn, int state, int x, int y);
	
	
	int width() const {return m_windowWidth;}
	int height() const {return m_windowHeight;}

protected:
    void drawHelp(const char* string) const;
    void drawHUD(const char* string) const;

	int m_mouseButton, m_mouseState;
    int m_mouseX, m_mouseY;
    int m_mousePressX, m_mousePressY;
    int m_windowWidth, m_windowHeight;
    bool m_showHelp, m_showHUD;
};

} // namespace OGL

#endif
