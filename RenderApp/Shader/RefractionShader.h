
#ifndef REFRACTION_SHADER_H
#define REFRACTION_SHADER_H

#include <vector>
#include "../Scene/Scene.h"
#include "RecursiveShader.h"

class RefractionShader : public RecursiveShader
{
protected:
	Math::Color3f m_kd;
	SurfaceShader* recursiveShader;
	float refractionPercentage;
	float refractionIndex;
	bool myShader;

	float reflectFresnel(float n1, float n2, float thetaI, float thetaT) const;
	Math::Color3f calculateRefraction(const HitInfo & hit, const Scene* scene, stack<float> refractionIndices, int recursionDepth, bool inside, Vec3f normal, float sinTheta2) const;
	Math::Color3f calculateReflection(const HitInfo & hit, const Scene* scene, stack<float> refractionIndices, int recursionDepth, bool inside, Vec3f normal) const;

public:
	RefractionShader(float refractionIndex, const Math::Color3f & kd = Math::Color3f(0.8f), float refractionPercentage = 1, int maxRecursionDepth = 5, SurfaceShader* recursiveShader = 0);
	virtual ~RefractionShader();
	
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const;
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices, int mirrorDepth) const;
	virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const;
};

#endif