
#include "PhongLobeShader.h"
#include "LambertShader.h"
#include "../Scene/LightRay.h"
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/PhongHemisphereWarping.h"
#include "../Scene/EnvironmentMap.h"

PhongLobeShader::PhongLobeShader(const Math::Color3f& kd, float mirrorPercentage_in, int maxRecursionDepth_in, SurfaceShader* recursiveShader_in, float exponent, unsigned int nSamples_in) :
		recursiveShader(recursiveShader_in), mirrorPercentage(mirrorPercentage_in), RecursiveShader(maxRecursionDepth_in), exponent(exponent) {
	nSamplesSqrt = round2Int(sqrt(static_cast<float>(nSamples_in)));
	nSamples = nSamplesSqrt*nSamplesSqrt;
	randomSampler = new RandomSampler();
	phongLobeWarping = new PhongHemisphereWarping(exponent);
	if (recursiveShader == 0) {
		myShader = true;
		recursiveShader = new LambertShader(kd);
	} else
		myShader = false;
}

PhongLobeShader::~PhongLobeShader() {
	delete randomSampler;
	delete phongLobeWarping;
	if (myShader)
		delete recursiveShader;
}

Color3f PhongLobeShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const {
	return shade(hit, scene, refractionIndices, 0);
}

Color3f PhongLobeShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices, int recursionDepth) const {
	if (recursionDepth < maxRecursionDepth) {
		std::vector<Light*> lights = scene->lights;
		Color3f color(0,0,0);
		if (mirrorPercentage < 1)
			color += (1-mirrorPercentage) * recursiveShader->shade(hit, scene, refractionIndices);
		
		Vec3f mirroredD = 2*dot(hit.N, -hit.I)*hit.N + hit.I;
		Mat44f rotation = Mat44f(1);
		rotation.rotateTo(Vec3f(0,0,1), mirroredD);

		std::vector<Vec2f> samples(nSamples);
		randomSampler->generateSamples(nSamplesSqrt, samples);

		for (size_t i = 0; i < nSamples; i++) {
			Ray r;
			r.o = hit.P;
			r.time = hit.time;
			r.d = rotation * phongLobeWarping->warp(samples[i]);

			//loop over all scene objects and find the closest intersection
			for (size_t k = 0; k < scene->shapes.size(); k++)
				scene->shapes[k]->intersect(&r);

			//if ray hit something then shade it
			if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
				const RecursiveShader* rShader = dynamic_cast<const RecursiveShader*>(r.hit.surfaceShader);
				if (rShader != 0)
					color += mirrorPercentage * rShader->shade(r.hit, scene, refractionIndices, recursionDepth+1);
				else
					color += mirrorPercentage * r.hit.surfaceShader->shade(r.hit, scene, refractionIndices);
			}
			else {
				Color4f background = scene->background->getBackground(r.d);
				color += mirrorPercentage * Vec3f(background.x, background.y, background.z);
			}
		}
	
		return color / static_cast<float>(nSamples);
	} else
		return recursiveShader->shade(hit, scene, refractionIndices);
}