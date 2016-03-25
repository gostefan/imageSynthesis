
#include "NoDisplacement.h"

NoDisplacement::NoDisplacement() : Displacement(1.f) {};
NoDisplacement::~NoDisplacement() {};
	
/// Returns a displacement in [-scale, scale]
float NoDisplacement::getDisplacement(float u, float v) {
	return 0;
}
float NoDisplacement::getDerivativeU(float u, float v) {
	return 0;
}
float NoDisplacement::getDerivativeV(float u, float v) {
	return 0;
}