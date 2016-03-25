//
//  ReyesRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef ReyesRenderer_h
#define ReyesRenderer_h

#include <OGL/FBO.h>
#include <Math/Box.h>
#include <queue>
#include <time.h>
#include <vector>

#include "../Scene/Scene.h"
#include "../Geometry/Shape.h"
#include "Renderer.h"

using namespace std;

class ReyesRenderer : public Renderer
{
protected:
	const float maxSize;
	const int nDice;

	virtual void setRes(int x, int y);

	Util::Array2D<Math::Color4f> m_rgbaBuffer;
	Util::Array2D<float> m_zBuffer;
	FrameBuffer * m_fbo;

	virtual void split(Scene& scene, vector<SurfacePatch*>& result);
	virtual void dice(vector<SurfacePatch*>& surfaces, vector<MicroGrid*>& result);
	virtual void shade(vector<MicroGrid*>& grids, Scene& scene);
	virtual void bust(vector<MicroGrid*>& grids, vector<MicroPolygon*>& result, Camera& camera);
	virtual void sample(vector<MicroPolygon*>& polygons);
	virtual void filter();
	
public:
	ReyesRenderer();
	~ReyesRenderer();
	
	virtual void render(Scene & scene);
	virtual void saveImage(std::string filename);

};

#endif
