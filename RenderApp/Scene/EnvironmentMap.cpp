
#include "EnvironmentMap.h"
#include <Img/ImageIO.h>
#include <Math/Vec2.h>

namespace {
	const double PI = 3.1415926539;
}

EnvironmentMap::EnvironmentMap(unsigned sizeX, unsigned sizeY, vector<vector<Color4f> > values) : 
		values(values), sizeX(sizeX), sizeY(sizeY) { }

EnvironmentMap::EnvironmentMap(const string &name) : sizeX(0), sizeY(0) {
	std::cout << "Reading Image data...\n";
	Math::Color3f* readColors = Img::readImage(name, &sizeX, &sizeY);

	std::cout << "Converting Image data...\n";
	values = vector<vector<Math::Color4f> >(sizeY);
	margPDF = vector<float>(sizeY);
	condPDF = vector<vector<float> >(sizeY);
	int pos = 0;
	for (int i = sizeY-1; i >= 0; i--) {
		float theta = static_cast<float>(PI)*((static_cast<float>(i) / (sizeY-1))-0.5f) ;
		values[i] = vector<Math::Color4f>(sizeX);
		condPDF[i] = vector<float>(sizeX);
		for (int j = 0; j < sizeX; j++) {
			values[i][j] = Math::Color4f(readColors[pos], 1);
			float intensity = readColors[pos].length() * cos(theta);
			margPDF[i] += intensity;
			condPDF[i][j] += intensity;
			pos++;
		}
	}

	std::cout << "Computing prefix sums.\n";
	// Do the first individually
	for (int j = 1; j < sizeX; j++) {
		condPDF[0][j] += condPDF[0][j-1];
	}
	for (int i = 1; i < sizeY; i++) {
		for (int j = 1; j < sizeX; j++) {
			condPDF[i][j] += condPDF[i][j-1];
		}
		margPDF[i] += margPDF[i-1];
	}
	intensitySum = margPDF[sizeY-1];

	std::cout << "Environment Map created.\n";
}

Color4f EnvironmentMap::getBackground(Vec3f direction) const {
	float phi = atan2(direction.z, direction.x);
	float theta = asin(direction.y);

	float posX = ((phi/static_cast<float>(2*PI))+0.5f)*(sizeX-1);
	float posY = ((theta/static_cast<float>(PI))+0.5f)*(sizeY-1);
	int lPosX = static_cast<int>(floor(posX));
	int hPosX = static_cast<int>(ceil(posX));
	int lPosY = static_cast<int>(floor(posY));
	int hPosY = static_cast<int>(ceil(posY));

	float linInPoX = posX - floor(posX);
	float linInPoY = posY - floor(posY);

	Color4f interpX1 = (1-linInPoX) * values[lPosY][lPosX] + linInPoX * values[lPosY][hPosX];
	Color4f interpX2 = (1-linInPoX) * values[hPosY][lPosX] + linInPoX * values[hPosY][hPosX];
	return (1-linInPoY) * interpX1 + linInPoY * interpX2;
}

Vec3f EnvironmentMap::importanceSample(Vec2f sample) const {
	// Find first pixel index
	float value1 = sample.x * intensitySum;
	int firstInd = findBisective(value1, margPDF, 0, sizeY-1);
	// Find second pixel index
	float value2 = 0;
	if (firstInd > 0)
		value2 = sample.y * (margPDF[firstInd] - margPDF[firstInd-1]);
	else
		value2 = margPDF[0];
	int secondInd = findBisective(value2, condPDF[firstInd], 0, sizeX-1);
	// Calculate direction
	float phi = static_cast<float>(2*PI)*((static_cast<float>(secondInd) / (sizeX-1))-0.5f);//posX = ((phi/static_cast<float>(2*PI))+0.5f)*(sizeX-1);
	float theta = static_cast<float>(PI)*((static_cast<float>(firstInd) / (sizeY-1))-0.5f) ;//posY = ((theta/static_cast<float>(PI))+0.5f)*(sizeY-1);
	return Vec3f(cos(phi)*cos(theta), sin(phi)*cos(theta), sin(theta));
}

int EnvironmentMap::findBisective(float value, const vector<float>& in, int min, int max) const {
	if (max - min <= 1) {
		if (in[min] > value)
			return min;
		else
			return max;
	}
	int index = (max - min) / 2 + min;
	if (in[index] > value)
		return findBisective(value, in, min, index);
	else
		return findBisective(value, in, index+1, max);
}