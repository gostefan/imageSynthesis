
#include "ShiftedSineDisplacement.h"

ShiftedSineDisplacement::ShiftedSineDisplacement(float scale, unsigned int uTimes_in, unsigned int vTimes_in) : MultipliedSineDisplacement(scale, uTimes_in, vTimes_in) {};
ShiftedSineDisplacement::~ShiftedSineDisplacement() {};
	
/// Returns a displacement in [-scale, scale]
float ShiftedSineDisplacement::getDisplacement(float u, float v) {
	return MultipliedSineDisplacement::getDisplacement(fmodf(u + v, 1), v);
}
float ShiftedSineDisplacement::getDerivativeU(float u, float v) {
	return MultipliedSineDisplacement::getDerivativeU(fmodf(u + v, 1), v);
}
float ShiftedSineDisplacement::getDerivativeV(float u, float v) {
	return MultipliedSineDisplacement::getDerivativeV(fmodf(u + v, 1), v);
}