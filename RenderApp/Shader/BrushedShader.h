
#pragma once

#include "PhongLobeShader.h"

#include "../Scene/Scene.h"

#include <vector>

class BrushedShader : public PhongLobeShader {
	protected:
		float exponent2;

	public:
		BrushedShader(const Math::Color3f& kd, float mirrorPercentage_in, int maxRecursionDepth_in, SurfaceShader* recursiveShader_in, float exponent1, float exponent2, unsigned int nSamples_in = 16);
		virtual ~BrushedShader();
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const;
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices, int mirrorDepth) const;
		virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};