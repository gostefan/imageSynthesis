
#include "Warping.h"

#include <Math/Vec2.h>
#include <Math/Vec3.h>

using namespace Math;

class UniformCylinderWarping : public Warping {
	public:
		virtual Vec3f warp(Vec2f sample);
		virtual float pdf(Vec2f coord);
};