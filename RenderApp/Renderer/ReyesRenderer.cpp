//
//  ReyesRenderer.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "ReyesRenderer.h"
#include <Img/ImageIO.h>

using namespace Math;

ReyesRenderer::ReyesRenderer() : maxSize(60.f), nDice(20) { // nice values are 20.f and 5
	m_fbo.reset(new FrameBuffer(GL_TEXTURE_2D, 512, 512, -1, GL_RGBA32F_ARB, 1, 1, 0, "Reyes FBO"));
	m_fbo->checkFramebufferStatus(1);
}

ReyesRenderer::~ReyesRenderer() { } // Needs to be here because of unique_ptr's dtor

void ReyesRenderer::render(Scene& scene) {
	setRes(scene.camera->xRes(), scene.camera->yRes());

	vector<SurfacePatch*> splitShapes;
	split(scene, splitShapes);

	vector<MicroGrid*> microGrids;
	dice(splitShapes, microGrids);

	shade(microGrids, scene);

	vector<MicroPolygon*> microPolygons;
	bust(microGrids, microPolygons, *scene.camera);

	sample(microPolygons);
	filter();

	cout << "Freeing: " << splitShapes.size() << " SurfacePatches, " << microGrids.size() << " Micro Grids, " << microPolygons.size() << " Micro Polygons.\n";

	//Copy the final rendering to the texture
	glBindTexture(GL_TEXTURE_2D, m_fbo->colorTextureID(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &m_rgbaBuffer(0,0));
	glBindTexture(GL_TEXTURE_2D, 0);
	
	m_fbo->blitFramebuffer(FBO_COLOR0);
}

void ReyesRenderer::split(Scene& scene, vector<SurfacePatch*>& result) {
	// Precompute some values
	clock_t start = clock();
	Mat44f worldToWindow = scene.camera->NDCToWindow() * scene.camera->worldToNDC();

	// First we split the objects into smaller pieces
	for (unsigned int i = 0; i < scene.shapes.size(); i++) {
		// exclude not renderable shapes
		if (!scene.shapes[i]->isRenderable()) {
			continue;
		}

		// reduce each shape to the required size
		std::list<SurfacePatch*> toSplit;
		toSplit.push_back(new SurfacePatch(scene.shapes[i]));
		while(!toSplit.empty()) {
			// Get the next element
			SurfacePatch* current = toSplit.front();
			toSplit.pop_front();

			// We get the bounding box and decide whether it's on the screen
			BBox currentBB = current->getBBox();
			currentBB.project(worldToWindow);
			if (currentBB.isOnScreen(scene.camera->yRes(), scene.camera->xRes())) {
				// Is it still too big?
				if (currentBB.size() > maxSize) {
					// Split in both directions
					std::list<SurfacePatch*> intermediate;
					current->split(intermediate, USplit);

					for (std::list<SurfacePatch*>::iterator iter = intermediate.begin(); iter != intermediate.end(); iter++)
						(*iter)->split(toSplit, VSplit);
					delete current;
				} else {
					// Store for the next phase
					result.push_back(current);
				}
			} else
				delete current;

			if (toSplit.size() > 100000) {
				cout << "To big displacement or too small max split size\n";
				throw 20;
			}
		}
	}
	cout << "Split in " << (static_cast<float>(clock() - start)/CLOCKS_PER_SEC) << " Seconds\n";
}

void ReyesRenderer::dice(vector<SurfacePatch*>& surfaces, vector<MicroGrid*>& result) {
	clock_t start = clock();
	// We generate grids from the object pieces
	for (unsigned int i = 0; i < surfaces.size(); i++) {
		MicroGrid* mg = new MicroGrid(surfaces[i]->shape, surfaces[i]->shape->surfaceShader);
		surfaces[i]->dice(mg, nDice, nDice);
		result.push_back(mg);
		delete surfaces[i];
	}
	cout << "Diced in " << (static_cast<float>(clock() - start)/CLOCKS_PER_SEC) << " Seconds\n";
}

void ReyesRenderer::shade(vector<MicroGrid*>& grids, Scene& scene) {
	clock_t start = clock();
	// We shade each vertex of the grids
	for (unsigned int i = 0; i < grids.size(); i++)
		grids[i]->shade(*scene.camera, &scene);
	cout << "Shaded in " << (static_cast<float>(clock() - start)/CLOCKS_PER_SEC) << " Seconds\n";
}

void ReyesRenderer::bust(vector<MicroGrid*>& grids, vector<MicroPolygon*>& result, Camera& camera) {
	clock_t start = clock();
	// Precompute some values
	Mat44f worldToWindow = camera.NDCToWindow() * camera.worldToNDC();

	// We extract the small polygons from the grid
	for (unsigned int i = 0; i < grids.size(); i++) {
		grids[i]->project(worldToWindow);
		grids[i]->bust(result);
		delete grids[i];
	}
	
	// We remove the polygons not on the screen
	for (unsigned int i = 0; i < result.size(); i++) {
		if (!result[i]->isOnScreen(camera.yRes(), camera.xRes())) {
			delete result[i];
			result.erase(result.begin() + i);
			i--;
		}
	}
	cout << "Busted in " << (static_cast<float>(clock() - start)/CLOCKS_PER_SEC) << " Seconds\n";
}

void ReyesRenderer::sample(vector<MicroPolygon*>& polygons) {
	clock_t start = clock();
	// We render the polygons onto the frame
	for (unsigned int i = 0; i < polygons.size(); i++) {
		polygons[i]->rasterize(m_rgbaBuffer, m_zBuffer);
		delete polygons[i];
	}
	cout << "Sampled in " << (static_cast<float>(clock() - start)/CLOCKS_PER_SEC) << " Seconds\n";
}

void ReyesRenderer::filter() {
	// We postprocess the final image
}

void ReyesRenderer::setRes(int x, int y) {
	m_rgbaBuffer.resizeErase(x, y);
	m_zBuffer.resizeErase(x, y);
	m_fbo->resizeExistingFBO(x, y);
	
	// clear the rgb and depth buffers
	m_rgbaBuffer = Vec4f(16.f/255.f, 52.f/255.f, 138.f/255.f, 1.f);
	m_zBuffer = 1.0e10f;
	
	// Upload a blank texture to the FBO
	glBindTexture(GL_TEXTURE_2D, m_fbo->colorTextureID(0));
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_fbo->width(), m_fbo->height(), GL_RGBA, GL_FLOAT, &m_rgbaBuffer(0,0));
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ReyesRenderer::saveImage(std::string filename) {
	Img::ImageData data (filename.append(".hdr"),m_rgbaBuffer);
	Img::writeImage(data);
}
