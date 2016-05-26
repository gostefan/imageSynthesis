
#pragma once

#include <Math/Vec2.h>
#include <Math/Vec3.h>

using namespace Math;

class Warping {
	public:
		virtual Vec3f warp(Vec2f sample) = 0;
		virtual float pdf(Vec2f coord) = 0;
};
