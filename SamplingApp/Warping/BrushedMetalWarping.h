
#ifndef PI
#define PI 3.1415926f
#endif

#pragma once

#include "Warping.h"

using namespace Math;

class BrushedMetalWarping : public Warping {
	public:
		BrushedMetalWarping(float exp1, float exp2);
		virtual Vec3f warp(Vec2f sample);
		virtual float pdf(Vec2f coord);
	protected:
		float exp1, exp2;
};