
#include "Motion.h"

Motion::Motion(Vec3f translation) 
	: completeTranslation(translation) { }

Vec3f Motion::getTranslation(float time) const {
	return time * completeTranslation;
}