/*! \file Renderer.h
 \author Wojciech Jarosz
 */
#ifndef RENDERER_H
#define RENDERER_H

#include "../Scene/Scene.h"
	
class Renderer
{
public:
	virtual ~Renderer() {}
	
	virtual void render(Scene & scene) = 0;

	virtual void saveImage(std::string filename) = 0;

};

#endif



