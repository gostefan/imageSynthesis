/*! \file Renderer.h
 \author Wojciech Jarosz
 */

#pragma once

#include "../Scene/Scene.h"
	
class Renderer {
	public:
		virtual ~Renderer() {}
	
		virtual void render(Scene& scene) = 0;
		virtual void saveImage(std::string filename) = 0;
};
