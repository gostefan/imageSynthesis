
#include "LambertShader.h"
#include "../Scene/LightRay.h"
#include "../Scene/photonmap.h"
#include "Sampling/Sampler/RandomSampler.h"
#include "Sampling/Warping/CosineHemisphereWarping.h"

#include <algorithm>


namespace {
	const float PI = 3.1415926f;
}

LambertShader::LambertShader(Texture* surfaceTexture_in) :
	SurfaceShader(surfaceTexture_in) { }

LambertShader::LambertShader(const Color3f & kd) : m_kd(kd), cosineScatter(kd) { }

LambertShader::~LambertShader() { } // Needs to be here because of the unique_ptr dtor

Color3f LambertShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const {
	std::vector<Light*> lights = scene->lights;
	Color3f color(0,0,0);
	if (!scene->usePMapDirect) {
		for (unsigned int i = 0; i < lights.size(); i++) {
			vector<LightRay> samples;
			lights[i]->getIrradianceSamples(hit.P, scene, samples, hit.time);
			Color3f estimate(0);
			for (unsigned int j = 0; j < samples.size(); j++) {
				float dotProd = std::max(dot(-samples[j].direction, hit.N), 0.f);
				if (surfaceTexture == 0)
					estimate += dotProd * samples[j].radiance * m_kd;
				else
					estimate += dotProd * samples[j].radiance * surfaceTexture->getDiffuse(hit);
			}
			color += estimate / (PI * static_cast<float>(samples.size()));
		}
	} else {
		Color3f irr;
		scene->pMap->irradiance_estimate(irr, hit.P.toArray(), hit.N.toArray(), scene->maxPhotonDist, scene->nEstimatePhotons);
		color = irr * m_kd / PI;
	}
	return color;
}

bool LambertShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	return cosineScatter.scatterPhoton(hit, photon, scene, continueProb);
}