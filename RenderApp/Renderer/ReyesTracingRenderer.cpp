//
//  ReyesRenderer.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "ReyesTracingRenderer.h"
#include <Img/ImageIO.h>

using namespace Math;

ReyesTracingRenderer::ReyesTracingRenderer() : ReyesRenderer() { }

ReyesTracingRenderer::~ReyesTracingRenderer() { }

void ReyesTracingRenderer::shade(vector<MicroGrid*>& grids, Scene& scene) {
	cout << "No Shading needed\n";
}

void ReyesTracingRenderer::sample(vector<MicroPolygon*>& polygons) {
	clock_t start = clock();
	// We render the polygons onto the frame
	for (unsigned int i = 0; i < polygons.size(); i++) {
		polygons[i]->rasterize(m_rgbaBuffer, m_zBuffer);
		delete polygons[i];
	}
	cout << "Sampled in " << (static_cast<float>(clock() - start)/CLOCKS_PER_SEC) << " Seconds\n";
}
