
#include "Sampling/Sampler/RandomSampler.h"
#include "Sampling/Warping/UniformSphereCapWarping.h"
#include "../Scene/Scene.h"
#include "SphereCapLight.h"

#include <algorithm>

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
	const float ONE_BY_TWO_PI = 1 / TWO_PI;
}

SphereCapLight::SphereCapLight(Math::Color3f power, SurfaceShader* surfaceShader, const Math::Vec3f& location, float radius, unsigned int nSamplesSqrt, Displacement* displacement) :
		Sphere(0, location, radius, 0, displacement), SphereLight(power, surfaceShader, location, radius, nSamplesSqrt, displacement) {
	randomSampler.reset(new RandomSampler());
}

SphereCapLight::~SphereCapLight() {
	SphereLight::~SphereLight();
};

Math::Color3f SphereCapLight::getPower() {
	return power;
}

void SphereCapLight::getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time) const {
	// Calculate cap heigth
	float connectionSq = (point - location).length2();
	float connection = sqrt(connectionSq);
	float tangentSq = connectionSq - radius*radius;
	float heightToPoint = tangentSq / connection;
	auto sphereWarping = std::unique_ptr<Warping>(new UniformSphereCapWarping((radius - (connection-heightToPoint)) / radius));

	// Calculate rotation
	Vec3f dir = (point - location).normalized();
	Mat44f rotation = Mat44f(1);
	rotation.rotateTo(Vec3f(0,0,1), dir);

	// Draw new samples
	std::vector<Vec2f> drawnPoints(nSamples);
	randomSampler->generateSamples(nSamplesSqrt, drawnPoints);
	
	Color3f sum = Color3f(0);
	for (unsigned int i = 0; i < nSamples; i++) {
		// Warp and rotate samples
		if (point.x < 0 && point.y > 0)
			int a = 0;
		Vec3f warpedPoint = rotation * sphereWarping->warp(drawnPoints[i]);
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
		for (unsigned int k = 0; k < scene->shapes.size(); k++) {
			scene->shapes[k]->intersect(&r);
		}

		//if ray hit something then it does not contribute
		if (r.hit.shape == 0) {
			float pdf = sphereWarping->pdf(drawnPoints[i]);
			//lr.radiance = power * dot(lr.direction, warpedPoint) / (sphereWarping->pdf(drawnPoints[i]) * 4*PI * pow(distance+radius, 2));
			float a = 1/(sphereWarping->pdf(drawnPoints[i]) * 4*PI);
			lr.radiance = radiance * 4 * pow(radius,2) * std::max(0.f,dot(lr.direction, warpedPoint)) / (sphereWarping->pdf(drawnPoints[i]) * 4*PI * pow(distance, 2));
		}
		else {
			lr.radiance = Color3f(0);
		}
		result.push_back(lr);
	}
}