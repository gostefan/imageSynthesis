
#include "UniformSphereCapWarping.h"

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
	const float ONE_BY_TWO_PI = 1 / TWO_PI;

	const char* NAME = "Uniform Sphere Cap";
}

UniformSphereCapWarping::UniformSphereCapWarping(float height) : capValue(height) {
	// We need values larger than zero since the pdf is infinity otherwise
	//assert(capValue > 0 && capValue <= 2);
}

Vec3f UniformSphereCapWarping::warp(Vec2f sample) const {
	float theta = TWO_PI * sample.y;
	Vec3f cyl = Vec3f(cos(theta), sin(theta), 1 - sample.x * capValue);

	float r = sqrt(1 - cyl.z * cyl.z);
	return Vec3f(r * cyl.x, r * cyl.y, cyl.z);
}

float UniformSphereCapWarping::pdf(Vec2f coord) const {
	return ONE_BY_TWO_PI / capValue;
}

const char* UniformSphereCapWarping::getName() const {
	return NAME;
}