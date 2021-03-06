//
//  ReyesRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#pragma once

#include "Renderer.h"

#include "../Scene/Scene.h"
#include "../Geometry/Shape.h"

#include <OGL/FBO.h>
#include <Math/Box.h>

#include "memory.h"
#include <queue>
#include <time.h>
#include <vector>

using namespace std;

using SurfacePatchVector = std::vector<SurfacePatchPtr>;

class ReyesRenderer : public Renderer {
	protected:
		const float maxSize;
		const int nDice;

		virtual void setRes(int x, int y);

		Util::Array2D<Math::Color4f> m_rgbaBuffer;
		Util::Array2D<float> m_zBuffer;
		std::unique_ptr<FrameBuffer> m_fbo;

		virtual void split(Scene& scene, SurfacePatchVector& result);
		virtual void dice(SurfacePatchVector& surfaces, MicroGridVector& result);
		virtual void shade(MicroGridVector& grids, Scene& scene);
		virtual void bust(MicroGridVector& grids, MicroPolygonVector& result, Camera& camera);
		virtual void sample(MicroPolygonVector& polygons);
		virtual void filter();
	
	public:
		ReyesRenderer();
		~ReyesRenderer();
	
		virtual void render(Scene & scene);
		virtual void saveImage(std::string filename);
};
