//
//  MultipliedMultipliedSineDisplacement.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/04/12
//  Copyright 2011 Student. All rights reserved.
//

#ifndef MULTIPLIED_SINE_DISPLACEMENT_H
#define MULTIPLIED_SINE_DISPLACEMENT_H

#ifndef PI
#define PI 3.1415926f
#endif

#include "Displacement.h"
#include <Math.h>

class MultipliedSineDisplacement : public Displacement
{
public:
	MultipliedSineDisplacement(float scale, unsigned int uTimes, unsigned int vTimes);
	~MultipliedSineDisplacement();
	
	/// Returns a displacement in [-scale, scale]
	virtual float getDisplacement(float u, float v);
	virtual float getDerivativeU(float u, float v);
	virtual float getDerivativeV(float u, float v);
protected:
	unsigned int uTimes, vTimes;
};

#endif
