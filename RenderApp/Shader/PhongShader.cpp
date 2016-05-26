
#include "../Scene/Scene.h"
#include "PhongShader.h"

#include <algorithm>

PhongShader::PhongShader(Texture* surfaceTexture_in, float shininess) :
		SurfaceShader(surfaceTexture_in), m_shininess(shininess) { }

PhongShader::PhongShader(const Color3f& kd,
						 const Color3f& ks,
						 float shininess) :
		m_kd(kd), m_ks(ks), m_shininess(shininess) { }

PhongShader::~PhongShader() { }

Color3f PhongShader::shade(const HitInfo& hit, const Scene* scene) const {
	std::vector<Light*> lights = scene->lights;
	Color3f color(0,0,0);
	for (unsigned int i = 0; i < lights.size(); i++) {
		Vec3f mirroredD = 2*dot(hit.N, -hit.I)*hit.N + hit.I;

		vector<LightRay> samples;
		lights[i]->getIrradianceSamples(hit.P, scene, samples, hit.time);
		Color3f estimate(0);

		for (unsigned int j = 0; j < samples.size(); j++) {
			float dotProd = std::max(dot(-samples[j].direction, hit.N), 0.f);
			if (surfaceTexture == 0) {
				estimate += dotProd * samples[j].radiance * m_kd; // Diffuse term
				estimate += std::max(0.0f, pow(dot(-samples[j].direction, mirroredD), m_shininess)) * samples[j].radiance * m_ks; // specular term
			} else {
				estimate += dotProd * samples[j].radiance * surfaceTexture->getDiffuse(hit); // Diffuse term
				estimate += std::max(0.0f, pow(dot(-samples[j].direction, mirroredD), m_shininess)) * samples[j].radiance * surfaceTexture->getSpecular(hit); // specular term
			}
		}
		color += estimate / static_cast<float>(samples.size());
	}
	return color;
}