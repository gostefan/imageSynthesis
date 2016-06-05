
#pragma once

#include <vector>
#include "../Scene/Scene.h"
#include "RecursiveShader.h"
#include "Sampling/Sampler/Sampler.h"
#include "Sampling/Warping/Warping.h"

class PhongLobeShader : public RecursiveShader {
	protected:
		Math::Color3f m_kd;
		SurfaceShader* recursiveShader;
		float mirrorPercentage;
		bool myShader;
		unsigned int nSamples;
		unsigned int nSamplesSqrt;
		float exponent;

		Sampler* randomSampler;
		Warping* phongLobeWarping;

	public:
		PhongLobeShader(const Math::Color3f& kd = Math::Color3f(0.8f), float mirrorPercentage = 1, int maxRecursionDepth = 2, SurfaceShader* recursiveShader = 0, float exponent = 100, unsigned int nSamples = 10);
		virtual ~PhongLobeShader();
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const;
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices, int mirrorDepth) const;
};