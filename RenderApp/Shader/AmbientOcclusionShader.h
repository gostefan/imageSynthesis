
#ifndef AMB_OCC_SHADER_H
#define AMB_OCC_SHADER_H

#include <Math/Color.h>
#include <Img/Fwd.h>
#include <vector>
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/UniformHemisphereWarping.h"
#include "SamplingApp/Warping/CosineHemisphereWarping.h"
#include "../Scene/HitInfo.h"
#include "LambertShader.h"

class Light;

using namespace Math;

class AmbientOcclusionShader : public SurfaceShader, private LambertShader {
public:
	AmbientOcclusionShader(const Math::Color3f & color = Math::Color3f(1), unsigned int nSamples = 10, bool cosineWeighted = true, bool envionmentMapWeighted = false);
	~AmbientOcclusionShader();
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const;
	virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
protected:
	Color3f color;
	bool cosineWeighted;
	bool environmentMapWeighted;
	unsigned int nSamples, nSamplesSqrt;
	Sampler* currentSampler;
	Warping* currentWarping;
};

#endif