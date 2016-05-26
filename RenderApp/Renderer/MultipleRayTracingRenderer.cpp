//
//  RayTracingRenderer.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include <OGL/FBO.h>
#include "MultipleRayTracingRenderer.h"
#include "../Shader/Shader.h"
#include "../Shader/PathTracingShader.h"
#include "../Scene/EnvironmentMap.h"

MultipleRayTracingRenderer::MultipleRayTracingRenderer(unsigned int nSamples) : 
		nSamples(nSamples), RayTracingRenderer() { }

MultipleRayTracingRenderer::~MultipleRayTracingRenderer() { }

void MultipleRayTracingRenderer::render(Scene& scene) {
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
			Color4f color = Color4f(0);
			for (unsigned int k = 0; k < nSamples; k++) {
				Ray r = Ray();
				scene.camera->generateRay(&r, static_cast<float>(i), static_cast<float>(j));
		
				//loop over all scene objects and find the closest intersection
				for (unsigned int k = 0; k < scene.shapes.size(); k++)
					scene.shapes[k]->intersect(&r);

				//if ray hit something then shade it
				if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
					std::stack<float> refraction = std::stack<float>();
					refraction.push(1);
					Math::Color3f shaded = r.hit.surfaceShader->shade(r.hit, &scene, refraction);
					color += Math::Color4f(shaded, 1);
				} else
					color += scene.background->getBackground(r.d);
			}
			color /= static_cast<float>(nSamples);
					
			m_rgbaBuffer(i,j).x = color.x;
			m_rgbaBuffer(i,j).y = color.y;
			m_rgbaBuffer(i,j).z = color.z;
			m_rgbaBuffer(i,j).w = 1;
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
	m_fbo->blitFramebuffer(FBO_COLOR0);
}
