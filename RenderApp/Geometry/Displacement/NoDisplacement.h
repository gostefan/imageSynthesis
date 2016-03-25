//
//  NoDisplacement.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/04/12
//  Copyright 2011 Student. All rights reserved.
//

#ifndef NO_DISPLACEMENT_H
#define NO_DISPLACEMENT_H

#include "Displacement.h"

class NoDisplacement : public Displacement
{
public:
	NoDisplacement();
	~NoDisplacement();
	
	/// Returns a displacement in [-scale, scale]
	virtual float getDisplacement(float u, float v);
	virtual float getDerivativeU(float u, float v);
	virtual float getDerivativeV(float u, float v);
};

#endif
