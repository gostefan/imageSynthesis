
#include "CheckerboardTexture.h"

CheckerboardTexture::CheckerboardTexture(float frequencyX_in, float frequencyY_in, float frequencyZ_in, Color3f color1_in, Color3f color2_in) : frequencyX(frequencyX_in), frequencyY(frequencyY_in), frequencyZ(frequencyZ_in), color1(color1_in), color2(color2_in) {};

Color3f CheckerboardTexture::getAmbient(const HitInfo& hit) {
	Vec3f position = hit.P;
	float n = fmod(position.x * 2 * frequencyX + position.y * 2 * frequencyY + position.z * 2 * frequencyZ, 2);
	return n > 1 ? color2 : color1;
}
Color3f CheckerboardTexture::getDiffuse(const HitInfo& hit) {
	return getAmbient(hit);
}
Color3f CheckerboardTexture::getSpecular(const HitInfo& hit) {
	return getAmbient(hit);
}