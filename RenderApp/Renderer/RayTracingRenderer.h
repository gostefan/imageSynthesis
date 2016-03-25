//
//  RayTracingRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef RayTracingRenderer_h
#define RayTracingRenderer_h

#include <OGL/FBO.h>
#include <Math/Vec4.h>
#include <Math/Box.h>
#include "Renderer.h"
#include <Platform/Progress.h>

class RayTracingRenderer : public Renderer
{
protected:
	void setRes(int x, int y);
	
	Util::Array2D<Math::Color4f> m_rgbaBuffer;
	FrameBuffer * m_fbo;
	Math::Box2i m_updatedRegion;
	
public:
	RayTracingRenderer();
	~RayTracingRenderer();
	
	virtual void render(Scene & scene);
	virtual void saveImage(std::string filename);

};

#endif
