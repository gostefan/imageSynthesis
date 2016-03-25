
#include <Math/Vec2.h>
#include <Math/Vec3.h>

#include "Warping.h"

using namespace Math;

class UniformSquareWarping : public Warping {
public:
	virtual Vec3f warp(Vec2f sample);
	virtual float pdf(Vec2f coord);
};