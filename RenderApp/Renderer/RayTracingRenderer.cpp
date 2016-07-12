//
//  RayTracingRenderer.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "RayTracingRenderer.h"

#include <Img/ImageIO.h>
#include <OGL/FBO.h>

#include "../Scene/EnvironmentMap.h"
#include "../Shader/Shader.h"

#include "util/memory.h"
#include <stack>


RayTracingRenderer::RayTracingRenderer() {
	m_fbo = new FrameBuffer(GL_TEXTURE_2D, 512, 512, -1, GL_RGBA32F_ARB, 1, 1, 0, "Ray Tracing FBO");
	m_fbo->checkFramebufferStatus(1);
}

RayTracingRenderer::~RayTracingRenderer() {
	delete m_fbo;
}

void RayTracingRenderer::render(Scene & scene) {
	setRes(scene.camera->xRes(), scene.camera->yRes());

	//clear m_rgbaBuffer
	this->m_rgbaBuffer(scene.camera->xRes(), scene.camera->yRes());
	this->m_rgbaBuffer.reset(Color4f(0));

	//setup progress reporting using Platform::Progress
	Platform::Progress renderProgress("Initializing", (scene.camera->xRes()/50)+1);

	//for each pixel generate a camera ray 
	unsigned int xRes = scene.camera->xRes();
	unsigned int yRes = scene.camera->yRes();
	for (unsigned int i = 0; i < xRes; i++) {
		for (unsigned int j = 0; j < yRes; j++) {
			auto r = std::unique_ptr<Ray>(new Ray());
			scene.camera->generateRay(r.get(), static_cast<float>(i), static_cast<float>(j));
	
			//loop over all scene objects and find the closest intersection
			for (unsigned int k = 0; k < scene.shapes.size(); k++)
				scene.shapes[k]->intersect(r.get());

			//if ray hit something then shade it
			if (r->hit.shape != 0 && r->hit.surfaceShader != 0) {
				std::stack<float> refraction = std::stack<float>();
				refraction.push(1);
				Math::Color3f shaded = r->hit.surfaceShader->shade(r->hit, &scene, refraction);
				
				m_rgbaBuffer(i,j).x = shaded.x;
				m_rgbaBuffer(i,j).y = shaded.y;
				m_rgbaBuffer(i,j).z = shaded.z;
				m_rgbaBuffer(i,j).w = 1;
			} else
				m_rgbaBuffer(i,j) = scene.background->getBackground(r->d);
		}

		if (i % 50 == 0)
			renderProgress.step();
	}
	renderProgress.step();

	//Copy the final rendering to the texture
	glBindTexture(GL_TEXTURE_2D, m_fbo->colorTextureID(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &m_rgbaBuffer(0,0));
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//Render to Screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_fbo->displayAsFullScreenTexture(FBO_COLOR0);
}


void RayTracingRenderer::setRes(int x, int y) {
	m_rgbaBuffer.resizeErase(x, y);
	m_fbo->resizeExistingFBO(x, y);
	
	// clear the buffers
	m_rgbaBuffer = Math::Vec4f(0.0f);
	
	// Upload a blank texture to the FBO
	glBindTexture(GL_TEXTURE_2D, m_fbo->colorTextureID(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &m_rgbaBuffer(0,0));
	glBindTexture(GL_TEXTURE_2D, 0);
}


void RayTracingRenderer::saveImage(std::string filename) {
	Img::ImageData data (filename.append(".hdr"),m_rgbaBuffer);
	Img::writeImage(data);
}
