
#pragma once

#include <vector>
#include "../Scene/Scene.h"
#include "Shader.h"
#include "../Geometry/Texture/Texture.h"

#include "util/memory.h"

class Sampler;
class Warping;

class PathTracingShader : public SurfaceShader {
	protected:
		static float eliminationProbability;
		SurfaceShader* innerShader;
		std::unique_ptr<Sampler> randomSampler;
		std::unique_ptr<Warping> cosineWarping;
		int nSamplesSqrt;
		bool shadowRays;
		Color3f shaderColor;

	public:
		PathTracingShader(SurfaceShader* innerShader, Color3f color, int nSamplesSqrt = 3, bool shadowRays = true, Texture* surfaceTexture = 0);
		//PathTracingShader(const Math::Color3f & kd = Math::Color3f(0.8f));
		virtual ~PathTracingShader();
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const;
		static void setEliminationProbability(float probability) {
			eliminationProbability = probability;
		};
		virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};