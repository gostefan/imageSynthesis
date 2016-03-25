//
//  Texture.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/16/12
//  Copyright 2012 Student. All rights reserved.
//

#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include <string>
#include <vector>

#include <Math/Color.h>
#include "../../Scene/HitInfo.h"

using namespace std;
using namespace Math;

class ImageTexture {
public:
	ImageTexture(const string& diffuseName, const string& specularName = "", const string& ambientName = "");
	virtual Color3f getAmbient(const HitInfo& hit);
	virtual Color3f getDiffuse(const HitInfo& hit);
	virtual Color3f getSpecular(const HitInfo& hit);

protected:
	// Interpolates a cubic polynomial through val0 (pos = -1), val1 (pos = 0), etc. and evaluates it at pos
	Color3f interpolate1D(float pos, Color3f val0, Color3f val1, Color3f val2, Color3f val3);
	// Interpolates bicubically in the corresponding texture
	Color3f interpolate2D(const HitInfo& hit, vector<vector<Color3f> >& texture, int sizeX, int sizeY);

	bool hasAmbient;
	vector<vector<Color3f> > ambient;
	// bool hasDiffuse; would be always true
	vector<vector<Color3f> > diffuse;
	bool hasSpecular;
	vector<vector<Color3f> > specular;
	int sizeAX, sizeAY, sizeDX, sizeDY, sizeSX, sizeSY;
};

#endif
