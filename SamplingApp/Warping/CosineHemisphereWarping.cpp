
#include "CosineHemisphereWarping.h"

Vec3f CosineHemisphereWarping::warp(Vec2f sample) {
	//Vec3f disk = UniformDiskWarping::warp(sample);
	float r = sqrt(sample.x);
	Vec3f disk = Vec3f(r * cos(2*PI * sample.y), r * sin(2*PI * sample.y), 0.f);

	float sum = disk.x*disk.x + disk.y*disk.y;
	if (sum < 1) {
		disk.z = sqrt(1 - sum);
	}
	return disk;
}

float CosineHemisphereWarping::pdf(Vec2f coord) {
	return sqrt(1-coord.x)/PI;
}