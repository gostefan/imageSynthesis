
#pragma once

#include <Math/Vec2.h>
#include <Math/Vec3.h>

using namespace Math;

class Warping {
	public:
		virtual Vec3f warp(Vec2f sample) const = 0;
		virtual float pdf(Vec2f coord) const = 0;
		virtual const char* getName() const = 0;
};
