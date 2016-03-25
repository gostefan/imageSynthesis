
#include "LambertShader.h"
#include "../Scene/LightRay.h"
#include "../Scene/photonmap.h"
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/CosineHemisphereWarping.h"

LambertShader::LambertShader(Texture* surfaceTexture_in) :
	SurfaceShader(surfaceTexture_in) { }

LambertShader::LambertShader(const Color3f & kd) :
	m_kd(kd)
{
	randomSampler = new RandomSampler();
	cosineWarping = new CosineHemisphereWarping();
}

LambertShader::~LambertShader()
{
	delete randomSampler;
	delete cosineWarping;
}

Color3f
LambertShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const
{
	std::vector<Light*> lights = scene->lights;
	Color3f color(0,0,0);
	if (!scene->usePMapDirect) {
		for (unsigned int i = 0; i < lights.size(); i++) {
			vector<LightRay> samples;
			lights[i]->getIrradianceSamples(hit.P, scene, samples, hit.time);
			Color3f estimate(0);
			for (unsigned int j = 0; j < samples.size(); j++) {
				float dotProd = max(dot(-samples[j].direction, hit.N), 0.f);
				if (surfaceTexture == 0) {
					estimate += dotProd * samples[j].radiance * m_kd;
				}
				else {
					estimate += dotProd * samples[j].radiance * surfaceTexture->getDiffuse(hit);
				}
			}
			color += estimate / (PI * static_cast<float>(samples.size()));
		}
	}
	else {
		Color3f irr;
		scene->pMap->irradiance_estimate(irr, hit.P.toArray(), hit.N.toArray(), scene->maxPhotonDist, scene->nEstimatePhotons);
		color = irr * m_kd / PI;
	}
	return color;
}

bool LambertShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	// store photon
	scene.pMap->store(photon.power, hit.P.toArray(), hit.I.toArray());

	// Calculate rotation
	Mat44f rot = Mat44f::I();
	rot.rotateTo(Vec3f(0,0,1), hit.N);

	// Calculate new direction
	Vec2f sample = Vec2f();
	randomSampler->generateSamples(1, &sample);
	photon.direction = rot * cosineWarping->warp(sample);
	
	// Set hit position and color
	photon.origin = hit.P;
	photon.power = photon.power * m_kd;

	// We decide whether the photon is scattered further depending on the reflectance value
	return (rand() / static_cast<float>(RAND_MAX)) < continueProb;
}