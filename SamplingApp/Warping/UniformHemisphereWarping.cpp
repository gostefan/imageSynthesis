
#include "UniformHemisphereWarping.h"

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
	const float ONE_BY_TWO_PI = 1 / TWO_PI;
}

Vec3f UniformHemisphereWarping::warp(Vec2f sample) {
	return UniformSphereWarping::warp(Vec2f(0.5f*sample.x + 0.5f, sample.y));
}

float UniformHemisphereWarping::pdf(Vec2f coord) {
	return ONE_BY_TWO_PI;
}