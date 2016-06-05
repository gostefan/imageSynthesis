
#pragma once

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include "Warping.h"

using namespace Math;

class CosineHemisphereWarping : public Warping {
public:
	virtual Vec3f warp(Vec2f sample) const override;
	virtual float pdf(Vec2f coord) const override;
	virtual const char* getName() const override;
};