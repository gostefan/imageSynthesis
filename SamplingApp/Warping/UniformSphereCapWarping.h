
#pragma once

#include "Warping.h"

#include <Math/Vec2.h>
#include <Math/Vec3.h>

using namespace Math;

class UniformSphereCapWarping : public Warping {
	public:
		UniformSphereCapWarping(float height);
		virtual Vec3f warp(Vec2f sample) const override;
		virtual float pdf(Vec2f coord) const override;
		virtual const char* getName() const override;

	private:
		float capValue;
};