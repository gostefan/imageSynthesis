
#include "BBox.h"

BBox::BBox(Vec3f lower, Vec3f upper) : lower(lower), upper(upper) {};
BBox::~BBox() {};

Vec3f BBox::getLower() {
	return lower;
};
Vec3f BBox::getUpper(){
	return upper;
};

void BBox::project(const Math::Mat44f& m) {
	dividedLower = Vec3f(Limits<float>::max());
	dividedUpper = Vec3f(Limits<float>::min());
	Vec3f cubeCorner;
	for (int i = 0; i < 8; i++) {
		cubeCorner.x = (i%2 == 0 ? lower.x : upper.x);
		cubeCorner.y = (i/2%2 == 0 ? lower.y : upper.y);
		cubeCorner.z = (i/4%2 == 0 ? lower.z : upper.z);
		cubeCorner = m * cubeCorner;
		cubeCorner /= cubeCorner.z;
		dividedLower.x = min(dividedLower.x, cubeCorner.x);
		dividedLower.y = min(dividedLower.y, cubeCorner.y);
		dividedUpper.x = max(dividedUpper.x, cubeCorner.x);
		dividedUpper.y = max(dividedUpper.y, cubeCorner.y);
	}
	dividedLower.z = 1;
	dividedUpper.z = 1;
}

bool BBox::isOnScreen(int height, int width) {
	return dividedUpper.x >= 0 && dividedUpper.y >= 0 && dividedLower.x < width && dividedLower.y < height;
}

float BBox::size() {
	return std::max(dividedUpper.x - dividedLower.x, dividedUpper.y - dividedLower.y);
}