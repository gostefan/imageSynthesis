//
//  OpenGLRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef OpenGLRenderer_h
#define OpenGLRenderer_h

#include "Renderer.h"
#include <Img/ImageIO.h>
#include <OGL/FBO.h>

class OpenGLRenderer : public Renderer
{
protected:
	void setRes(int x, int y);
	FrameBuffer * m_fbo;

public:
	OpenGLRenderer();
	~OpenGLRenderer();

	virtual void render(Scene & scene);
	virtual void saveImage(std::string filename);

};

#endif
