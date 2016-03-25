
#include "UniformCylinderWarping.h"

Vec3f UniformCylinderWarping::warp(Vec2f sample) {
	float theta = 2*PI * sample.y;
	return Vec3f(cos(theta), sin(theta), sample.x);
}

float UniformCylinderWarping::pdf(Vec2f coord) {
	return 1/(2*PI);
}