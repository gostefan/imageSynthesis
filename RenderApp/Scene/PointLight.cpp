
#include "PointLight.h"
#include "../Shader/Shader.h"
#include "Scene.h"
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/UniformSphereWarping.h"

#include <Math/Box.h>
#include <Math/Vec3.h>

namespace {
	const float PI = 3.1415926f;
	const float FOUR_PI = 4 * PI;
}

PointLight::PointLight(const Math::Vec3f & position, Math::Color3f & power ) : m_position(position), m_power(power) {
	randomSampler = std::unique_ptr<Sampler>(new RandomSampler());
	sphereWarping = std::unique_ptr<Warping>(new UniformSphereWarping());
}

PointLight::~PointLight() {
	delete surfaceShader;
}

bool PointLight::isRenderable() const {
    return false;
}

bool PointLight::intersect(Ray* r) const {
	return false;
}

void PointLight::fillHitInfo(Ray* r) const { }

Math::Color3f PointLight::getPower() {
	return m_power;
}

Math::Vec3f PointLight::getPosition() {
	return m_position;
}

Vec3f PointLight::evalP(float u, float v) const {
	return Vec3f(0.0f);
}

Vec3f PointLight::evalN(float u, float v) const {
	return Vec3f(0.0f);
}

void PointLight::renderGL() const { }

BBox PointLight::getBBox() const {
	return BBox(m_position, m_position);
};

BBox PointLight::getBBox(float uStart, float uEnd, float vStart, float vEnd) const {
	return BBox(m_position, m_position);
};

void PointLight::getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time) {
	LightRay lr;
	Ray r;
	lr.direction = (point - m_position).normalized();
	lr.radiance = m_power / (point - m_position).length2();

	float distance = (point - m_position).length();
	
	r.d = lr.direction;
	r.o = m_position;
	r.tMax = distance - r.tMin;
	r.time = time;

	// Check for intersection
	for (unsigned int k = 0; k < scene->shapes.size(); k++)
		scene->shapes[k]->intersect(&r);

	//if ray hit something then it does not contribute
	if (r.hit.shape == 0)
		lr.radiance = m_power / (FOUR_PI * pow(distance, 2));
	else
		lr.radiance = Color3f(0);

	result.push_back(lr);
}

TracePhoton PointLight::samplePhoton() {
	std::vector<Vec2f> sample(1);
	randomSampler->generateSamples(1, sample);
	
	Vec3f warpedPoint = m_position;
	Vec3f warpedDirection = sphereWarping->warp(sample.front());

	TracePhoton photon = TracePhoton(warpedPoint, warpedDirection, m_power / (FOUR_PI));

	return photon;
}