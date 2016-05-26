//
//  Texture.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/16/12
//  Copyright 2012 Student. All rights reserved.
//

#pragma once

#include <Math/Color.h>

class HitInfo;

using namespace Math;

class Texture {
	public:
		virtual Color3f getAmbient(const HitInfo& hit) = 0;
		virtual Color3f getDiffuse(const HitInfo& hit) = 0;
		virtual Color3f getSpecular(const HitInfo& hit) = 0;
};
