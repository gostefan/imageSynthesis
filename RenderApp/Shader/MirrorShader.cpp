
#include "MirrorShader.h"
#include "LambertShader.h"
#include "../Scene/LightRay.h"
#include "../Scene/EnvironmentMap.h"

MirrorShader::MirrorShader(const Math::Color3f& kd, float mirrorPercentage_in, int maxRecursionDepth_in, SurfaceShader* recursiveShader_in) :
	recursiveShader(recursiveShader_in), mirrorPercentage(mirrorPercentage_in), RecursiveShader(maxRecursionDepth_in) {
	if (recursiveShader == nullptr) {
		myShader = true;
		recursiveShader = new LambertShader(kd);
	}
	else {
		myShader = false;
	}
}

MirrorShader::~MirrorShader() {
	if (myShader)
		delete recursiveShader;
}

Color3f MirrorShader::shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices) const {
	return shade(hit, scene, refractionIndices, 0);
}

Color3f MirrorShader::shade(const HitInfo& hit, const Scene* scene, stack<float>& refractionIndices, int recursionDepth) const {
	if (recursionDepth < maxRecursionDepth) {
		std::vector<Light*> lights = scene->lights;
		Color3f color(0,0,0);
		if (mirrorPercentage < 1)
			color += (1-mirrorPercentage) * recursiveShader->shade(hit, scene, refractionIndices);
		
		Ray r;
		r.time = hit.time;
		r.o = hit.P;
		r.d = 2*dot(hit.N, -hit.I)*hit.N + hit.I;

		//loop over all scene objects and find the closest intersection
		for (unsigned int k = 0; k < scene->shapes.size(); k++)
			scene->shapes[k]->intersect(&r);

		//if ray hit something then shade it
		if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
			const RecursiveShader* rShader = dynamic_cast<const RecursiveShader*>(r.hit.surfaceShader);
			if (rShader != 0)
				color += mirrorPercentage * rShader->shade(r.hit, scene, refractionIndices, recursionDepth+1);
			else
				color += mirrorPercentage * r.hit.surfaceShader->shade(r.hit, scene, refractionIndices);
		} else {
			Color4f background = scene->background->getBackground(r.d);
			color += mirrorPercentage * Vec3f(background.x, background.y, background.z);
		}
	
		return color;
	} else
		return recursiveShader->shade(hit, scene, refractionIndices);
}

bool MirrorShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	// Mirror photon
	photon.origin = hit.P;
	photon.direction = 2*dot(hit.N, -hit.I) * hit.N + hit.I;

	// We decide whether the photon is scattered further depending on the reflectance value
	return (rand() / static_cast<float>(RAND_MAX)) < mirrorPercentage;
}