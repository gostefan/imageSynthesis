//
//  OpenGLRenderer.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "OpenGLRenderer.h"
#include <Math/MathGL.h>
#include <Img/ImageIO.h>
#include "../Scene/photonmap.h"


OpenGLRenderer::OpenGLRenderer()
{
	m_fbo = new FrameBuffer(GL_TEXTURE_2D, 512, 512, -1, GL_RGBA32F_ARB, 1, 1, 0, "OpenGL FBO");
	m_fbo->checkFramebufferStatus(1);
}

OpenGLRenderer::~OpenGLRenderer()
{
	delete m_fbo;
}

void
OpenGLRenderer::render(Scene & scene)
{
	setRes(scene.camera->xRes(), scene.camera->yRes());
	m_fbo->bindBuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.camera->renderGL();
	
	for (unsigned int i = 0; i < scene.shapes.size(); ++i)
	{
		scene.shapes[i]->renderGL();
	}
	scene.pMap->renderGL();
	m_fbo->unbindBuffer();
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_fbo->displayAsFullScreenTexture(FBO_COLOR0);
}

void
OpenGLRenderer::setRes(int x, int y)
{
	m_fbo->resizeExistingFBO(x, y);
}



void 
OpenGLRenderer::saveImage(std::string filename) {
	
	Util::Array2D<Math::Color4f> rgbaBuffer(m_fbo->width(), m_fbo->height());

    //glReadBuffer(CPUbuffer);
    glReadPixels(0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &(rgbaBuffer(0,0)));
	
	Img::ImageData data (filename.append(".hdr"),rgbaBuffer);
	Img::writeImage(data);
}
