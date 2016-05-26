
#pragma once

#include <vector>
#include "../Scene/Scene.h"
#include "RecursiveShader.h"

class MirrorShader : public RecursiveShader {
	protected:
		Math::Color3f m_kd;
		SurfaceShader* recursiveShader;
		float mirrorPercentage;
		bool myShader;

	public:
		MirrorShader(const Math::Color3f& kd = Math::Color3f(0.8f), float mirrorPercentage = 1, int maxRecursionDepth = 5, SurfaceShader* recursiveShader = 0);
		virtual ~MirrorShader();
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const;
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices, int mirrorDepth) const;
		virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};