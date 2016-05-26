//
//  SineDisplacement.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/04/12
//  Copyright 2011 Student. All rights reserved.
//

#pragma once

#include "Displacement.h"
#include <Math.h>

class SineDisplacement : public Displacement {
	public:
		SineDisplacement(float scale, unsigned int uTimes, unsigned int vTimes);
		~SineDisplacement();
	
		/// Returns a displacement in [-scale, scale]
		virtual float getDisplacement(float u, float v);
		virtual float getDerivativeU(float u, float v);
		virtual float getDerivativeV(float u, float v);
	protected:
		unsigned int uTimes, vTimes;
};
