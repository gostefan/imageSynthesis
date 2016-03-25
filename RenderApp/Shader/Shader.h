//
//  Shader.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H

#include <Math/Color.h>
#include <Img/Fwd.h>
#include <vector>
#include <stack>
#include "../Scene/HitInfo.h"
#include "../Scene/Light.h"

using namespace std;

class Scene;

class SurfaceShader
{
public:
	SurfaceShader(Texture* surfaceTexture = 0);
	virtual ~SurfaceShader();

	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const = 0;
	virtual bool scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const = 0;
protected:
	Texture* surfaceTexture;
};

#endif
