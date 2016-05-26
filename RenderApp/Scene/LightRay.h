
#pragma once

#include <Math/Color.h>
#include <Math/Vec3.h>

struct LightRay {
	Math::Vec3f direction;
	Math::Color3f radiance;
};