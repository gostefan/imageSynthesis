//
//  Displacement.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/04/12
//  Copyright 2011 Student. All rights reserved.
//

#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H

class Displacement
{
protected:
	float scale;
	
public:
	Displacement(float scale = 1.f);
	~Displacement();
	
	/// Returns a displacement in [-scale, scale]
	virtual float getDisplacement(float u, float v) = 0;
	virtual float getDerivativeU(float u, float v) = 0;
	virtual float getDerivativeV(float u, float v) = 0;
	float getScale();
	void setScale(float scale);
};

#endif
