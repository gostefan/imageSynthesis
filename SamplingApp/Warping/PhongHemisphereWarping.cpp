
#include "PhongHemisphereWarping.h"

PhongHemisphereWarping::PhongHemisphereWarping(float n_in) : n(n_in) {
	//assert(n >= 0 && n <= 1);
}

Vec3f PhongHemisphereWarping::warp(Vec2f sample) {
	float theta = acos( pow(1-sample.x, 1.f/(n+1)));
	float phi = 2*PI* sample.y;
	return Vec3f(cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta));
}

float PhongHemisphereWarping::pdf(Vec2f coord) {
	float cosTheta = pow(1-coord.x, 1.f/(n+1));
	return (n+1)/(2*PI) * pow(cosTheta, n);
}