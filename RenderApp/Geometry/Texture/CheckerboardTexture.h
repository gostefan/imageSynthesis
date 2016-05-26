//
//  CheckerboardTexture.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/16/12
//  Copyright 2012 Student. All rights reserved.
//

#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "math.h"
#include "Texture.h"

class CheckerboardTexture : public Texture
{
public:
	CheckerboardTexture(float frequencyX = 1, float frequencyY = 1, float frequencyZ = 1, Color3f color1 = Color3f(1, 1, 1), Color3f color2 = Color3f(0, 0, 0));
	
	/// Returns a displacement in [-scale, scale]
	virtual Color3f getAmbient (const HitInfo& hit);
	virtual Color3f getDiffuse (const HitInfo& hit);
	virtual Color3f getSpecular(const HitInfo& hit);
protected:
	float frequencyX, frequencyY, frequencyZ;
	Color3f color1, color2;
};

#endif
