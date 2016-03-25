
#include "UniformHemisphereWarping.h"

Vec3f UniformHemisphereWarping::warp(Vec2f sample) {
	return UniformSphereWarping::warp(Vec2f(0.5f*sample.x + 0.5f, sample.y));
}

float UniformHemisphereWarping::pdf(Vec2f coord) {
	return 1/(2*PI);
}