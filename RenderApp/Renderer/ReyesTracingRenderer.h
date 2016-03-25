//
//  ReyesRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef ReyesTracingRenderer_h
#define ReyesTracingRenderer_h

#include <OGL/FBO.h>
#include <Math/Box.h>
#include <queue>
#include <time.h>
#include <vector>

#include "../Scene/Scene.h"
#include "../Geometry/Shape.h"
#include "ReyesRenderer.h"

using namespace std;

class ReyesTracingRenderer : public ReyesRenderer
{
protected:
	//void setRes(int x, int y);

	Util::Array2D<Math::Color4f> m_rgbaBuffer;
	Util::Array2D<float> m_zBuffer;
	FrameBuffer * m_fbo;

	//void split(Scene& scene, vector<SurfacePatch*>& result);
	//void dice(vector<SurfacePatch*>& surfaces, vector<MicroGrid*>& result);
	virtual void shade(vector<MicroGrid*>& grids, Scene& scene);
	//void bust(vector<MicroGrid*>& grids, vector<MicroPolygon*>& result, Camera& camera);
	virtual void sample(vector<MicroPolygon*>& polygons);
	//void filter();
	
public:
	ReyesTracingRenderer();
	~ReyesTracingRenderer();
	
	//virtual void render(Scene & scene);
	//virtual void saveImage(std::string filename);

};

#endif
