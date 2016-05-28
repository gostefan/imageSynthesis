//
//  SamplingApp.h
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Mat44.h>
#include <OGL/GfxGLUTWindow.h>

#include <math.h>
#include <memory>
#include <vector>

class Sampler;
class Warping;

class SamplingApp : public OGL::GfxGLUTWindow {
	public:
		SamplingApp(OGL::GLUTMaster* glutMaster, int setWidth, int setHeight,
					const char* title);
		~SamplingApp();
    
		void update();
	
		void display(void);
		void reshape(int w, int h);
		void keyboard(unsigned char key, int x, int y);
		void mouse(int button, int state, int x, int y);
		void motion(int x, int y);

		unsigned int getNPoints();
		void setNPoints(unsigned int nPoints);
    
	private:
		std::unique_ptr<Sampler> mSampler;
		std::unique_ptr<Warping> mWarping;
		size_t mValueCalc;
		unsigned int capValue;
		unsigned int nValue;
		unsigned int nPoints;
		unsigned int nPointsSqrt;
		std::vector<Math::Vec2f> inputPoints;
		std::unique_ptr<Math::Vec3f[]> warpedPoints;
		void resetView();
		void drawGrid(int res);
		Math::Vec3f warpPoint(Math::Vec2f);
		void generateSamples();
		void drawSamples();
		void warpSamples();
	
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
    
		bool m_drawGrid;
};
