
#include "LightShader.h"

LightShader::LightShader(const Math::Color3f& radiance, SurfaceShader* surfaceShader_in) :
		m_kd(radiance), surfaceShader(surfaceShader_in) { }

LightShader::~LightShader() { }

Color3f
LightShader::shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const {
	Color3f color = Color3f(0);
	if (surfaceShader != 0)
		color += surfaceShader->shade(hit, scene, refractionIndices);
	color += dot(hit.N, hit.I) < 0 ? m_kd : Color3f(0);
	return color;
}

bool LightShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	return false;
}