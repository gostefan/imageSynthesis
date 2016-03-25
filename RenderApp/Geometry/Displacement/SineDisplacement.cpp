
#include "SineDisplacement.h"

SineDisplacement::SineDisplacement(float scale, unsigned int uTimes_in, unsigned int vTimes_in) : Displacement(scale), uTimes(uTimes_in), vTimes(vTimes_in) {};
SineDisplacement::~SineDisplacement() {};
	
/// Returns a displacement in [-scale, scale]
float SineDisplacement::getDisplacement(float u, float v) {
	if (uTimes > 0 && vTimes > 0)
		return (sin(u * uTimes * 2 * PI) + sin(v * vTimes * 2 * PI)) * scale;
	if (uTimes > 0)
		return sin(u * uTimes * 2 * PI) * scale;
	if (vTimes > 0)
		return sin(v * vTimes * 2 * PI) * scale;
	return 0; // Should never occurr
}
float SineDisplacement::getDerivativeU(float u, float v) {
	return -cos(u * uTimes * 2 * PI) * scale;
}
float SineDisplacement::getDerivativeV(float u, float v) {
	return -cos(v * vTimes * 2 * PI) * scale;
}