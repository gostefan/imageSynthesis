
#pragma once

#include<vector>
#include<Math/color.h>

using namespace std;
using namespace Math;

class EnvironmentMap {
	public:
		EnvironmentMap(unsigned sizeX, unsigned sizeY, vector<vector<Color4f> > values);
		EnvironmentMap(const string &name);
		Color4f getBackground(Vec3f direction) const;
		Vec3f importanceSample(Vec2f sample) const;
	private:
		int findBisective(float value, const vector<float>& in, int min, int max) const;
		vector<vector<Color4f>> values;
		vector<float> margPDF;
		vector<vector<float>> condPDF;
		float intensitySum;
		int sizeX, sizeY;
};