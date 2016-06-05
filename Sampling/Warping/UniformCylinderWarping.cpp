
#include "UniformCylinderWarping.h"

namespace {
	const char* NAME = "Uniform Cylinder";
}

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
	const float ONE_BY_TWO_PY = 1 / TWO_PI;
}

Vec3f UniformCylinderWarping::warp(Vec2f sample) const {
	float theta = TWO_PI * sample.y;
	return Vec3f(cos(theta), sin(theta), sample.x);
}

float UniformCylinderWarping::pdf(Vec2f coord) const {
	return ONE_BY_TWO_PY;
}

const char* UniformCylinderWarping::getName() const {
	return NAME;
}