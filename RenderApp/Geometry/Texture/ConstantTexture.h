//
//  ConstantTexture.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/16/12
//  Copyright 2012 Student. All rights reserved.
//

#ifndef CONST_TEXTURE_H
#define CONST_TEXTURE_H

#include "math.h"
#include "Texture.h"

class ConstantTexture : public Texture
{
public:
	ConstantTexture(Color3f color = Color3f(1, 1, 1));
	~ConstantTexture();
	
	/// Returns a displacement in [-scale, scale]
	virtual Color3f getAmbient(const HitInfo& hit);
	virtual Color3f getDiffuse(const HitInfo& hit);
	virtual Color3f getSpecular(const HitInfo& hit);
protected:
	Color3f color;
};

#endif
