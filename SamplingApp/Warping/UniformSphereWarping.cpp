
#include "UniformSphereWarping.h"

Vec3f UniformSphereWarping::warp(Vec2f sample) {
	float r = sqrt(1 - (2 * sample.x - 1) * (2 * sample.x - 1));
	float theta = 2*PI * sample.y;
	return Vec3f(r * cos(theta), r * sin(theta), 2 * sample.x - 1);
}

float UniformSphereWarping::pdf(Vec2f coord) {
	return 1/(4*PI);
}