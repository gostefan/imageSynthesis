
#include "UniformSquareWarping.h"

Vec3f UniformSquareWarping::warp(Vec2f sample) {
	return Vec3f(2*sample.x - 1, 2*sample.y - 1, 0.f);
}

float UniformSquareWarping::pdf(Vec2f coord) {
	return 1.f;
}