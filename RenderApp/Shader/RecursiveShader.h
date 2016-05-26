
#pragma once

#include <vector>
#include "../Scene/Scene.h"
#include "Shader.h"

class RecursiveShader : public SurfaceShader {
	protected:
		int maxRecursionDepth;

	public:
		RecursiveShader(int maxRecursionDepth = 5) : maxRecursionDepth(maxRecursionDepth) { };
		virtual ~RecursiveShader() { };
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices, int recursionDepth) const = 0;
};