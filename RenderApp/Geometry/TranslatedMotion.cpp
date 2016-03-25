
#include "TranslatedMotion.h"

TranslatedMotion::TranslatedMotion(Vec3f translation1, Vec3f translation2) :
	Motion(translation2), translation1(translation1) { }

Vec3f TranslatedMotion::getTranslation(float t) const {
	return translation1 + t * completeTranslation;
}
