
#include "PathTracingShader.h"
#include "LightShader.h"
#include "../Scene/LightRay.h"
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/CosineHemisphereWarping.h"
#include "../Scene/EnvironmentMap.h"

namespace {
	const float PI = 3.141592653f;
}

float PathTracingShader::eliminationProbability;

PathTracingShader::PathTracingShader(SurfaceShader* innerShader, Color3f color, int nSamplesSqrt, bool shadowRays, Texture* surfaceTexture_in) :
		SurfaceShader(0), innerShader(innerShader), nSamplesSqrt(nSamplesSqrt), shadowRays(shadowRays), shaderColor(color) { 
	// If there is no surface Shader we can't shade properly
	if (!innerShader)
		abort();
	eliminationProbability = 0.5;
	randomSampler = std::unique_ptr<Sampler>(new RandomSampler());
	cosineWarping = std::unique_ptr<Warping>(new CosineHemisphereWarping());
}

PathTracingShader::~PathTracingShader() { } // Needs to be here because of unique_ptr dtor

Color3f PathTracingShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const {
	// Get normal rotation
	Mat44f rotation = Mat44f::I();
	rotation.rotateTo(Vec3f(0,0,1), hit.N);

	Color3f color = Color3f(0,0,0);

	// Only do the recursion if the probability matches
	if ((rand() / static_cast<float>(RAND_MAX)) > eliminationProbability) {
		// Generate sample
		std::vector<Vec2f> sample(1);
		randomSampler->generateSamples(1, sample);
	
		// Generate Ray
		Ray r = Ray();
		r.o = hit.P;
		r.time = hit.time;
		r.d = rotation * cosineWarping->warp(sample.front());

		//loop over all scene objects and find the closest intersection
		for (unsigned int k = 0; k < scene->shapes.size(); k++)
			scene->shapes[k]->intersect(&r);

		// See whether we hit something
		if (r.hit.shape != 0) {
			const LightShader* lShader = dynamic_cast<const LightShader*>(r.hit.surfaceShader);
			if (lShader && shadowRays)
				return shade(hit, scene, refractionIndices);
			else
				color += r.hit.surfaceShader->shade(r.hit, scene, refractionIndices);
		} else {
			// Add background to the estimate
			Color4f background = scene->background->getBackground(r.d);
			color += Color3f(background.x, background.y, background.z);
		}
		color *= shaderColor / (PI * (1-eliminationProbability));
	}

	// Do Surface Shading
	if (shadowRays)
		color += innerShader->shade(hit, scene, refractionIndices);

	return color;
}

bool PathTracingShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	return false;
}