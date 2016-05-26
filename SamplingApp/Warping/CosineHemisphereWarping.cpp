
#include "CosineHemisphereWarping.h"

namespace {
	const float PI = 3.1415926f;
	const float TWO_PI = 2 * PI;
}

Vec3f CosineHemisphereWarping::warp(Vec2f sample) {
	float r = sqrt(sample.x);
	Vec3f disk = Vec3f(r * cos(TWO_PI * sample.y), r * sin(TWO_PI * sample.y), 0.f);

	float sum = disk.x*disk.x + disk.y*disk.y;
	if (sum < 1) {
		disk.z = sqrt(1 - sum);
	}
	return disk;
}

float CosineHemisphereWarping::pdf(Vec2f coord) {
	return sqrt(1 - coord.x) / PI;
}