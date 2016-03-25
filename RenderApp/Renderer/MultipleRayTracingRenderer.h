//
//  MultipleRayTracingRenderer.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef MRayTracingRenderer_h
#define MRayTracingRenderer_h

#include "RayTracingRenderer.h"

class MultipleRayTracingRenderer : public RayTracingRenderer
{
protected:
	unsigned int nSamples;
public:
	MultipleRayTracingRenderer(unsigned int nSamples);
	~MultipleRayTracingRenderer();
	
	virtual void render(Scene & scene);
};

#endif
