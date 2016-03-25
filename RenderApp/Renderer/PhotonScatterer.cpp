#include "PhotonScatterer.h"
#include "../Shader/Shader.h"

#include <Math.h>

#include "../Scene/photonmap.h"

PhotonScaterer::PhotonScaterer(float continueProb) : continueProb(continueProb) {}

PhotonScaterer::~PhotonScaterer() {}

void PhotonScaterer::scatter(Scene & scene) {
	tracePhotons(scene);
}

void PhotonScaterer::tracePhotons(Scene& scene) {
	while (scene.pMap->getStoredPhotons() < scene.nPhotons) {
		// Todo: Implement better weighting between the lights so all the photon powers are equal.
		int l = round2Int(scene.lights.size() * (rand() / static_cast<float>(RAND_MAX))) % scene.lights.size();
		TracePhoton photon = scene.lights[l]->samplePhoton();
		photon.specular = false;
		photon.refractionIndex.push(1);

		bool isContinue = false;
		do {
			isContinue = false;

			// Calculate intersections
			Ray r = Ray(photon.origin, photon.direction);
			for (unsigned int i = 0; i < scene.shapes.size(); i++) {
				scene.shapes[i]->intersect(&r);
			}

			// See whether it intersected
			if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
				isContinue = r.hit.surfaceShader->scatterPhoton(r.hit, photon, scene, continueProb);
			}
		}
		while (isContinue && scene.pMap->getStoredPhotons() < scene.nPhotons);
	}
	scene.pMap->scale_photon_power(1/static_cast<float>(scene.pMap->getStoredPhotons()));
}