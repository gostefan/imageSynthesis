
#include "BrushedMetalWarping.h"

BrushedMetalWarping::BrushedMetalWarping(float exp1, float exp2) : exp1(exp1), exp2(exp2) {
	//assert(n >= 0 && n <= 1);
}

Vec3f BrushedMetalWarping::warp(Vec2f sample) {
	// Decide in which quarter the vector should lie
	bool mirror1 = sample.x > 0.5f ? 1 : 0;
	sample.x = mirror1 ? (sample.x - 0.5f)*2 : sample.x*2;
	bool mirror2 = sample.y > 0.5f ? 1 : 0;
	sample.y = mirror2 ? (sample.y - 0.5f)*2 : sample.y*2;

	// Find the angles
	float theta1 = acos( pow(sample.x, 1.f/(exp1+1)));
	float theta2 = acos( pow(sample.y, 1.f/(exp2+1)));

	// Define the vector
	Vec3f result = Vec3f(sin(theta1)*cos(theta2), sin(theta2)*cos(theta1), 1);

	return Vec3f(mirror1 ? -result.x : result.x, mirror2 ? -result.y : result.y, sqrt(1-pow(result.x,2)-pow(result.y,2)));
}

float BrushedMetalWarping::pdf(Vec2f coord) {
	float cosTheta1 = pow(1-coord.x, 1.f/(exp1+1));
	float cosTheta2 = pow(1-coord.y, 1.f/(exp2+1));
	return (exp1+1)/(2*PI) * pow(cosTheta1, exp1) * (exp2+1)/(2*PI) * pow(cosTheta2, exp2);
}