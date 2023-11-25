
#include "AmbientOcclusionShader.h"

#include "Sampling/Sampler/RandomSampler.h"
#include "Sampling/Warping/CosineHemisphereWarping.h"
#include "Sampling/Warping/UniformHemisphereWarping.h"

#include "../Scene/Scene.h"
#include "../Scene/EnvironmentMap.h"

AmbientOcclusionShader::AmbientOcclusionShader(const Color3f & color_in, unsigned int nSamples_in, bool cosineWeighted_in, bool envionmentMapWeighted)
		: color(color_in), cosineWeighted(cosineWeighted_in), nSamples(nSamples_in), environmentMapWeighted(environmentMapWeighted) {
	nSamplesSqrt = round2Int(sqrt(static_cast<float>(nSamples)));
	nSamples = nSamplesSqrt*nSamplesSqrt;

	currentSampler = std::unique_ptr<Sampler>(new RandomSampler());
	currentWarping = std::unique_ptr<Warping>(cosineWeighted ? static_cast<Warping*>(new CosineHemisphereWarping()) : static_cast<Warping*>(new UniformHemisphereWarping));
}

Color3f AmbientOcclusionShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const {
	// Draw new samples
	std::vector<Vec2f> drawnPoints(nSamples);
	currentSampler->generateSamples(nSamplesSqrt, drawnPoints);
	
	// Calculate rotation
	Mat44f rotation = Mat44f(1);
	rotation.rotateTo(Vec3f(0,0,1), hit.N);

	Color3f sum = Color3f(0);
	for (size_t i = 0; i < nSamples; i++) {
		Vec3f warpedPoint;
		// Include the environment Map weighting
		if (environmentMapWeighted) {
			warpedPoint = scene->background->importanceSample(drawnPoints[i]);
			while (dot(warpedPoint, hit.N) < 0) {
				std::vector<Vec2f> sample(1);
				currentSampler->generateSamples(1, sample);
				warpedPoint = scene->background->importanceSample(sample.front());
			}
		}
		else {
			// Warp and rotate samples
			warpedPoint = rotation * currentWarping->warp(drawnPoints[i]);
		}
		Ray r(hit.P, warpedPoint);
		r.time = hit.time;

		// Check for intersection
		for (unsigned int k = 0; k < scene->shapes.size(); k++) {
			scene->shapes[k]->intersect(&r);
		}

		//if ray hit something then it does not contribute
		if (r.hit.shape == 0) {
			if (cosineWeighted)
				sum += Vec3f(scene->background->getBackground(r.d).toArray());
			else
				sum += Vec3f(scene->background->getBackground(r.d).toArray()) * dot(warpedPoint, hit.N);
		}
	}

	if (!cosineWeighted)
		sum *= 2;

	return sum / static_cast<float>(nSamples);
}

bool AmbientOcclusionShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	return cosineScatter.scatterPhoton(hit, photon, scene, continueProb);
}