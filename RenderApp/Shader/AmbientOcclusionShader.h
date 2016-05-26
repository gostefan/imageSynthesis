
#ifndef AMB_OCC_SHADER_H
#define AMB_OCC_SHADER_H

#include "../Scene/HitInfo.h"
#include "LambertShader.h"

#include <Math/Color.h>
#include <Img/Fwd.h>

#include <memory>

class Light;

using namespace Math;

class AmbientOcclusionShader : public SurfaceShader, private LambertShader {
public:
	AmbientOcclusionShader(const Math::Color3f& color = Math::Color3f(1), unsigned int nSamples = 10, bool cosineWeighted = true, bool envionmentMapWeighted = false);
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const;
	virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
protected:
	Color3f color;
	bool cosineWeighted;
	bool environmentMapWeighted;
	unsigned int nSamples, nSamplesSqrt;
	std::unique_ptr<Sampler> currentSampler;
	std::unique_ptr<Warping> currentWarping;
};

#endif