
#include "UniformSphereCapWarping.h"

UniformSphereCapWarping::UniformSphereCapWarping(float height) : capValue(height) {
	// We need values larger than zero since the pdf is infinity otherwise
	//assert(capValue > 0 && capValue <= 2);
}

Vec3f UniformSphereCapWarping::warp(Vec2f sample) {
	float theta = 2*PI * sample.y;
	Vec3f cyl = Vec3f(cos(theta), sin(theta), 1 - sample.x * capValue);

	float r = sqrt(1 - cyl.z*cyl.z);
	return Vec3f(r * cyl.x, r * cyl.y, cyl.z);
}

float UniformSphereCapWarping::pdf(Vec2f coord) {
	return 1/(2*PI*capValue);
}