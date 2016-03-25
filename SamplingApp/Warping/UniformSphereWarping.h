
#ifndef UNI_SPHERE_WARPING_H
#define UNI_SPHERE_WARPING_H

#ifndef PI
#define PI 3.1415926f
#endif

#include <Math/Vec2.h>
#include <Math/Vec3.h>

#include "Warping.h"

using namespace Math;

class UniformSphereWarping : public Warping {
public:
	virtual Vec3f warp(Vec2f sample);
	virtual float pdf(Vec2f coord);
};

#endif