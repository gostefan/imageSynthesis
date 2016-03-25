
#ifndef LAMBERT_SHADER_H
#define LAMBERT_SHADER_H

#include <vector>
#include "../Scene/Scene.h"
#include "Shader.h"
#include "../Geometry/Texture/Texture.h"

class Sampler;
class Warping;

class LambertShader : public SurfaceShader
{
protected:
	Math::Color3f m_kd;
	Sampler* randomSampler;
	Warping* cosineWarping;

public:
	LambertShader(Texture* surfaceTexture);
	LambertShader(const Math::Color3f & kd = Math::Color3f(0.8f));
	virtual ~LambertShader();
	
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const;
	virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};

#endif