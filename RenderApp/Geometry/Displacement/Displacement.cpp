
#include "Displacement.h"

Displacement::Displacement(float scale) : scale(scale) { };

float Displacement::getScale() {
	return scale;
};

void Displacement::setScale(float scale) {
	this->scale = scale;
};