
#include "UniformSphereWarping.h"

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
	const float FOUR_PI = 4 * PI;
	const float ONE_BY_FOUR_PI = 1 / FOUR_PI;

	const char* NAME = "Uniform Sphere";
}

Vec3f UniformSphereWarping::warp(Vec2f sample) const {
	float r = sqrt(1 - (2 * sample.x - 1) * (2 * sample.x - 1));
	float theta = TWO_PI * sample.y;
	return Vec3f(r * cos(theta), r * sin(theta), 2 * sample.x - 1);
}

float UniformSphereWarping::pdf(Vec2f coord) const {
	return ONE_BY_FOUR_PI;
}

const char* UniformSphereWarping::getName() const {
	return NAME;
}