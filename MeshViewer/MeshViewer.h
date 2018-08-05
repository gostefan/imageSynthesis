//
//  MeshViewer.h
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/MeshBase.h"
#include "Math/Mat44.h"

#include "OGL/GfxGLUTWindow.h"

#include <vector>

class MeshViewer : public OGL::GfxGLUTWindow {
	public:
		MeshViewer(OGL::GLUTMaster& glutMaster, int setWidth, int setHeight,
				   const char* title, const Math::MeshBase& mesh);
		~MeshViewer();
    
		void update();

		void display(void) override;
		void reshape(int w, int h) override;
		void keyboard(unsigned char key, int x, int y) override;
		void motion(int x, int y) override;
		void mouse(int button, int state, int x, int y) override;
    
	private:
	
		void resetView();
		void drawGrid(int res);
	
		enum MouseMode {
			MM_NULL,
			MM_ROTATE,
			MM_ZOOM,
			MM_MOVE,
			MM_SELECT
		} m_mouseMode;
	
		struct Camera {
			float incline, azimuth, distance;
			Math::Vec3f center;
			Math::Mat44f matrix;
		} m_camera;
		void resetCamera();

		const Math::MeshBase& mesh;
    
		bool m_drawGrid;
};