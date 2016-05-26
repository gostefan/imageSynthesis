
#include "ConstantTexture.h"

ConstantTexture::ConstantTexture(Color3f color_in) :
		color(color_in) { }

Color3f ConstantTexture::getAmbient(const HitInfo& hit) {
	return color;
}
Color3f ConstantTexture::getDiffuse(const HitInfo& hit) {
	return color;
}
Color3f ConstantTexture::getSpecular(const HitInfo& hit) {
	return color;
}