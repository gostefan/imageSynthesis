
#include "SphereLight.h"
#include "../Shader/LightShader.h"


namespace {
	const double PI = 3.1415926f;
}

SphereLight::SphereLight(Math::Color3f power_in, SurfaceShader* surfaceShader, const Math::Vec3f& location, float radius, unsigned int nSamplesSqrt_in, Displacement* displacement) : 
Sphere(0, location, radius, 0, displacement), nSamplesSqrt(nSamplesSqrt_in), nSamples(nSamplesSqrt_in*nSamplesSqrt_in), power(power_in) {
	radiance = power/(4*PI * pow(radius, 2));
	Shape::surfaceShader = new LightShader(radiance, surfaceShader);
}

SphereLight::~SphereLight() {
	delete surfaceShader;
}

Math::Color3f SphereLight::getPower() {
	return power;
}

Math::Vec3f SphereLight::getPosition() {
	return location;
}