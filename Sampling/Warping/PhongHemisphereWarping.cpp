
#include "PhongHemisphereWarping.h"

namespace {
	const char* NAME = "Phong Hemisphere";
}

PhongHemisphereWarping::PhongHemisphereWarping(float n_in) : n(n_in) {
	//assert(n >= 0 && n <= 1);
}

Vec3f PhongHemisphereWarping::warp(Vec2f sample) const {
	float theta = acos( pow(1-sample.x, 1.f/(n+1)));
	float phi = 2*PI* sample.y;
	return Vec3f(cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta));
}

float PhongHemisphereWarping::pdf(Vec2f coord) const {
	float cosTheta = pow(1-coord.x, 1.f/(n+1));
	return (n+1)/(2*PI) * pow(cosTheta, n);
}

const char* PhongHemisphereWarping::getName() const {
	return NAME;
}