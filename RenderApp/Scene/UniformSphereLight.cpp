
#include "UniformSphereLight.h"

#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/UniformSphereWarping.h"
#include "../Scene/Scene.h"

#include <algorithm>

UniformSphereLight::UniformSphereLight(Math::Color3f power, SurfaceShader* surfaceShader, const Math::Vec3f& location, float radius, unsigned int nSamplesSqrt, Displacement* displacement) :
		Sphere(0, location, radius, 0, displacement), SphereLight(power, surfaceShader, location, radius, nSamplesSqrt, displacement) {
	randomSampler = std::unique_ptr<Sampler>(new RandomSampler());
	sphereWarping = std::unique_ptr<Warping>(new UniformSphereWarping());
}

UniformSphereLight::~UniformSphereLight() { };

Math::Color3f UniformSphereLight::getPower() {
	return power;
}

void UniformSphereLight::getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time) {
	// Draw new samples
	std::vector<Vec2f> drawnPoints(nSamples);
	randomSampler->generateSamples(nSamplesSqrt, drawnPoints);
	
	Color3f sum = Color3f(0);
	for (unsigned int i = 0; i < nSamples; i++) {
		// Warp and rotate samples
		Vec3f warpedPoint = sphereWarping->warp(drawnPoints[i]);
		Vec3f spherePoint = warpedPoint * radius;

		Vec3f difference = point - (spherePoint + location);
		float distance = difference.length();

		LightRay lr;
		Ray r;
		lr.direction = difference / distance;
		r.d = lr.direction;
		r.o = spherePoint + location;
		r.tMax = distance - r.tMin;
		r.time = time;

		// Check for intersection
		for (unsigned int k = 0; k < scene->shapes.size(); k++)
			scene->shapes[k]->intersect(&r);

		//if ray hit something then it does not contribute
		if (r.hit.shape == 0) {
			float pdf = sphereWarping->pdf(drawnPoints[i]);
			float a = std::max(0.f,dot(lr.direction, warpedPoint));
			float b = pow(radius+distance, 2);
			//lr.radiance = radiance * max(0.f,dot(lr.direction, warpedPoint)) /* (pdf/pow(radius, 2))*/ / pow(distance, 2);
			lr.radiance = radiance * 4 * pow(radius,2) * max(0.f,dot(lr.direction, warpedPoint)) / pow(distance, 2);
		} else
			lr.radiance = Color3f(0);
		result.push_back(lr);
	}
}