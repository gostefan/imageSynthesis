
#include "UniformSquareWarping.h"

namespace {
	const char* NAME = "Uniform Square";
}

Vec3f UniformSquareWarping::warp(Vec2f sample) const {
	return Vec3f(2*sample.x - 1, 2*sample.y - 1, 0.f);
}

float UniformSquareWarping::pdf(Vec2f coord) const {
	return 1.f;
}

const char* UniformSquareWarping::getName() const {
	return NAME;
}