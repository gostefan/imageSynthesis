
#ifndef SAMPLER_H
#define SAMPLER_H

#include <Math/Vec2.h>

class Sampler {
public:
	virtual void generateSamples(int nSamplesSqrt, Math::Vec2f* sampleStorage) = 0;
};

#endif