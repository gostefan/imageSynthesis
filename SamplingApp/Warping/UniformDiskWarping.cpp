
#include "UniformDiskWarping.h"

Vec3f UniformDiskWarping::warp(Vec2f sample) {
	float r = sqrt(sample.x);
	return Vec3f(r * cos(2*PI * sample.y), r * sin(2*PI * sample.y), 0.f);
}

float UniformDiskWarping::pdf(Vec2f coord) {
	return 1/PI;
}