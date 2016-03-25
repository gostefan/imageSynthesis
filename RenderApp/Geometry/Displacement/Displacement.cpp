
#include "Displacement.h"

Displacement::Displacement(float scale) : scale(scale) {};

Displacement::~Displacement() {};

float Displacement::getScale() {
	return scale;
};

void Displacement::setScale(float in_scale) {
	scale = in_scale;
};