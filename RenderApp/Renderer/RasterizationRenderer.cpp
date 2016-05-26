//
//  RasterizationRenderer.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "RasterizationRenderer.h"
#include <Img/ImageIO.h>

#include <OGL/FBO.h>
#include <Platform/Progress.h>

using namespace Math;


RasterizationRenderer::RasterizationRenderer() {
	m_fbo = new FrameBuffer(GL_TEXTURE_2D, 512, 512, -1, GL_RGBA32F_ARB, 1, 1, 0, "Rasterization FBO");
	m_fbo->checkFramebufferStatus(1);
}

RasterizationRenderer::~RasterizationRenderer() {
	delete m_fbo;
}

void RasterizationRenderer::render(Scene& scene) {
	setRes(scene.camera->xRes(), scene.camera->yRes());

	std::list<SurfacePatch*> patches;
	for (unsigned int i = 0; i < scene.shapes.size(); i++)
		if (scene.shapes[i]->isRenderable())
			scene.shapes[i]->split(patches, SurfacePatch(scene.shapes[i]), USplit);

	std::list<SurfacePatch*> splitPatches;
	for (std::list<SurfacePatch*>::iterator iter = patches.begin(); iter != patches.end(); iter++)
		(*iter)->split(splitPatches, VSplit);

	for (int i = 1; i <= 3; i++) {
		patches.clear();
		for (std::list<SurfacePatch*>::iterator iter = splitPatches.begin(); iter != splitPatches.end(); iter++) {
			(*iter)->split(patches, USplit);
		}

		splitPatches.clear();
		for (std::list<SurfacePatch*>::iterator iter = patches.begin(); iter != patches.end(); iter++) {
			(*iter)->split(splitPatches, VSplit);
		}
	}

	std::list<MicroGrid*> gridList;
	for (std::list<SurfacePatch*>::iterator iter = splitPatches.begin(); iter != splitPatches.end(); iter++) {
		MicroGrid* mg = new MicroGrid((*iter)->shape, (*iter)->shape->surfaceShader);
		gridList.push_back(mg);
		(*iter)->dice(mg, 2, 2);
	}

	Platform::Progress renderProgress("Raterizing", gridList.size());
	
	m_zBuffer.reset(Math::Limits<float>::max());
	for (std::list<MicroGrid*>::iterator iter = gridList.begin(); iter != gridList.end(); iter++) {
		(*iter)->shade(*scene.camera, &scene);
		(*iter)->project(scene.camera->NDCToWindow() * scene.camera->worldToNDC());
		(*iter)->rasterize(m_rgbaBuffer, m_zBuffer);
		renderProgress.step();
	}

	//Copy the final rendering to the texture
	glBindTexture(GL_TEXTURE_2D, m_fbo->colorTextureID(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &m_rgbaBuffer(0,0));
	glBindTexture(GL_TEXTURE_2D, 0);
	
	m_fbo->blitFramebuffer(FBO_COLOR0);
}


void RasterizationRenderer::setRes(int x, int y) {
	m_rgbaBuffer.resizeErase(x, y);
	m_zBuffer.resizeErase(x, y);
	m_fbo->resizeExistingFBO(x, y);
	
	// clear the rgb and depth buffers
	m_rgbaBuffer = Vec4f(0.0f);
	m_zBuffer = 1.0e10f;
	
	// Upload a blank texture to the FBO
	glBindTexture(GL_TEXTURE_2D, m_fbo->colorTextureID(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &m_rgbaBuffer(0,0));
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RasterizationRenderer::saveImage(std::string filename) {
	Img::ImageData data (filename.append(".hdr"), m_rgbaBuffer);
	Img::writeImage(data);
}
