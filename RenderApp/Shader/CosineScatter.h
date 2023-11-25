#pragma once

#include "../Scene/HitInfo.h"
#include "../Scene/Light.h"
#include "../Scene/photonmap.h"
#include "Sampling/Sampler/RandomSampler.h"
#include "Sampling/Warping/CosineHemisphereWarping.h"

#include <memory>

class CosineScatter {
private:
	Math::Color3f m_kd;
	std::unique_ptr<Sampler> randomSampler = std::unique_ptr<Sampler>(new RandomSampler());
	std::unique_ptr<Warping> cosineWarping = std::unique_ptr<Warping>(new CosineHemisphereWarping());
public:
	CosineScatter(Math::Color3f kd = Math::Color3f(0.8f)) : m_kd(kd) {}

	bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
		// store photon
		scene.pMap->store(photon.power, hit.P.toArray(), hit.I.toArray());

		// Calculate rotation
		Mat44f rot = Mat44f::I();
		rot.rotateTo(Vec3f(0,0,1), hit.N);

		// Calculate new direction
		std::vector<Vec2f> sample(1);
		randomSampler->generateSamples(1, sample);
		photon.direction = rot * cosineWarping->warp(sample.front());
		
		// Set hit position and color
		photon.origin = hit.P;
		photon.power = photon.power * m_kd;

		// We decide whether the photon is scattered further depending on the reflectance value
		return (rand() / static_cast<float>(RAND_MAX)) < continueProb;
	}
};