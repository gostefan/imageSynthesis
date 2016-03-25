//
//  RasterizationRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef RasterizationRenderer_h
#define RasterizationRenderer_h

#include "Renderer.h"
#include <Math/Box.h>
#include <Math/Color.h>
#include <OGL/FBO.h>
#include <Platform/Progress.h>

class RasterizationRenderer : public Renderer
{
protected:
	void setRes(int x, int y);
	
	Util::Array2D<Math::Color4f> m_rgbaBuffer;
	Util::Array2D<float> m_zBuffer;
	FrameBuffer * m_fbo;
	
public:
	RasterizationRenderer();
	~RasterizationRenderer();
	
	virtual void render(Scene & scene);
	virtual void saveImage(std::string filename);

};

#endif
