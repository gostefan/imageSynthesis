
#ifndef BRUSHED_SHADER_H
#define BRUSHED_SHADER_H

#include <vector>
#include "../Scene/Scene.h"
#include "PhongLobeShader.h"

class BrushedShader : public PhongLobeShader
{
protected:
	float exponent2;
	Warping* phongLobeWarping2;

public:
	BrushedShader(const Math::Color3f& kd, float mirrorPercentage_in, int maxRecursionDepth_in, SurfaceShader* recursiveShader_in, float exponent1, float exponent2, unsigned int nSamples_in = 16);
	virtual ~BrushedShader();
	
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const;
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices, int mirrorDepth) const;
	virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};

#endif