//
//  MeshViewer.h
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ImageSynthesisFramework_MeshViewer_h
#define ImageSynthesisFramework_MeshViewer_h

#include <vector>
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/MeshBase.h"
#include "Math/Mat44.h"
#include <OGL/GfxGLUTWindow.h>

class MeshViewer : public OGL::GfxGLUTWindow
{
public:
    MeshViewer(OGL::GLUTMaster * glutMaster, int setWidth, int setHeight,
			   const char * title, Math::MeshBase * mesh);
    ~MeshViewer();
    
    void update();
	
    void display(void);
    void reshape(int w, int h);
    void keyboard(unsigned char key, int x, int y);
    void motion(int x, int y);
    void mouse(int button, int state, int x, int y);
    
private:
	
	void resetView();
	void drawGrid(int res);
	
    enum MouseMode
    {
        MM_NULL,
        MM_ROTATE,
        MM_ZOOM,
        MM_MOVE,
        MM_SELECT
    } m_mouseMode;
	
	struct Camera
	{
		float incline, azimuth, distance;
		Math::Vec3f center;
		Math::Mat44f matrix;
	} m_camera;

	Math::MeshBase * m_mesh;
    
    bool m_drawGrid;
};

#endif
