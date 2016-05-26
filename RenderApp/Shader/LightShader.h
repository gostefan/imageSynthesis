
#pragma once

#include "../Scene/Scene.h"
#include "Shader.h"

class LightShader : public SurfaceShader {
	protected:
		Math::Color3f m_kd;
		SurfaceShader* surfaceShader;

	public:
		LightShader(const Math::Color3f& radiance, SurfaceShader* surfaceShader);
		virtual ~LightShader();
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const;
		virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};