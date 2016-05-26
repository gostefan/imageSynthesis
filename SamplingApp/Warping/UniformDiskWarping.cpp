
#include "UniformDiskWarping.h"

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
	const float ONE_BY_PI = 1 / PI;
}

Vec3f UniformDiskWarping::warp(Vec2f sample) {
	float r = sqrt(sample.x);
	return Vec3f(r * cos(TWO_PI * sample.y), r * sin(TWO_PI * sample.y), 0.f);
}

float UniformDiskWarping::pdf(Vec2f coord) {
	return ONE_BY_PI;
}