//
//  OpenGLRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#pragma once

#include "Renderer.h"
#include <Img/ImageIO.h>
#include <OGL/FBO.h>

#include <memory>

class OpenGLRenderer : public Renderer {
	protected:
		void setRes(int x, int y);
		std::unique_ptr<FrameBuffer> m_fbo;

	public:
		OpenGLRenderer();
		~OpenGLRenderer();

		virtual void render(Scene & scene);
		virtual void saveImage(std::string filename);
};