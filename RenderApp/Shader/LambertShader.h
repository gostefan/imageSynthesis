
#pragma once

#include <vector>
#include "../Scene/Scene.h"
#include "Shader.h"
#include "../Geometry/Texture/Texture.h"

#include "util/memory.h"

class Sampler;
class Warping;

class LambertShader : public SurfaceShader {
	protected:
		Math::Color3f m_kd;
		std::unique_ptr<Sampler> randomSampler;
		std::unique_ptr<Warping> cosineWarping;

	public:
		LambertShader(Texture* surfaceTexture);
		LambertShader(const Math::Color3f& kd = Math::Color3f(0.8f));
		virtual ~LambertShader();
	
		virtual Math::Color3f shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const;
		virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};