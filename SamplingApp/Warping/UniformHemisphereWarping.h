
#ifndef PI
#define PI 3.1415926f
#endif

#include <Math/Vec2.h>
#include <Math/Vec3.h>

#include "UniformSphereWarping.h"

using namespace Math;

class UniformHemisphereWarping : public UniformSphereWarping {
public:
	virtual Vec3f warp(Vec2f sample);
	virtual float pdf(Vec2f coord);
};