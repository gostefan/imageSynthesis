
#include "BrushedShader.h"

#include "LambertShader.h"
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/BrushedMetalWarping.h"
#include "../Scene/EnvironmentMap.h"
#include "../Scene/LightRay.h"

BrushedShader::BrushedShader(const Math::Color3f& kd, float mirrorPercentage_in, int maxRecursionDepth_in, SurfaceShader* recursiveShader_in, float exponent1, float exponent2, unsigned int nSamples_in) :
		PhongLobeShader(kd, mirrorPercentage_in, maxRecursionDepth_in, recursiveShader_in, exponent1, nSamples_in), exponent2(exponent2) {
	delete phongLobeWarping;
	phongLobeWarping = new BrushedMetalWarping(exponent1, exponent2);
	if (recursiveShader == 0) {
		myShader = true;
		recursiveShader = new LambertShader(kd);
	} else
		myShader = false;
}

BrushedShader::~BrushedShader() {
	delete randomSampler;
	delete phongLobeWarping;
	if (myShader)
		delete recursiveShader;
}

Color3f BrushedShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const {
	return shade(hit, scene, refractionIndices, 0);
}

Color3f BrushedShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices, int recursionDepth) const {
	if (recursionDepth < maxRecursionDepth) {
		std::vector<Light*> lights = scene->lights;
		Color3f color(0,0,0);
		if (mirrorPercentage < 1)
			color += (1-mirrorPercentage) * recursiveShader->shade(hit, scene, refractionIndices);
		
		Vec3f mirroredD = 2*dot(hit.N, -hit.I)*hit.N + hit.I;
		Mat44f rotation = Mat44f(1);
		Mat44f rotation2 = Mat44f(1);
		rotation.rotateTo(Vec3f(1,0,0), hit.dPdu);
		rotation2.rotateTo(rotation * Vec3f(0,0,1), mirroredD);
		rotation = rotation2 * rotation;
		//rotation.rotateTo(Vec3f(0,0,1), mirroredD);

		std::vector<Vec2f> samples(nSamples);
		randomSampler->generateSamples(nSamplesSqrt, samples);

		for (unsigned int i = 0; i < nSamples; i++) {
			Ray r;
			r.o = hit.P;
			r.time = hit.time;
			r.d = rotation * phongLobeWarping->warp(samples[i]);

			// Correct self-intersecting normals
			if (dot(r.d, hit.Ng) < 0)
				r.d = r.d - 1.01*dot(r.d, hit.N) * hit.N; // 1.01 is to go 1% away from the surface

			//loop over all scene objects and find the closest intersection
			for (unsigned int k = 0; k < scene->shapes.size(); k++)
				scene->shapes[k]->intersect(&r);

			//if ray hit something then shade it
			if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
				const RecursiveShader* rShader = dynamic_cast<const RecursiveShader*>(r.hit.surfaceShader);
				if (rShader != 0) {
					color += mirrorPercentage * rShader->shade(r.hit, scene, refractionIndices, recursionDepth+1);
				}
				else {
					color += mirrorPercentage * r.hit.surfaceShader->shade(r.hit, scene, refractionIndices);
				}
			} else {
				Color4f background = scene->background->getBackground(r.d);
				color += mirrorPercentage * Vec3f(background.x, background.y, background.z);
			}
		}
	
		return color / static_cast<float>(nSamples);
	} else
		return recursiveShader->shade(hit, scene, refractionIndices);
}

bool BrushedShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	return false;
}