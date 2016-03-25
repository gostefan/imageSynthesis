//
//  Shader.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "Shader.h"
#include "../Scene/Scene.h"

using namespace Img;
using namespace Math;
using namespace std;

SurfaceShader::SurfaceShader(Texture* surfaceTexture_in) :
surfaceTexture(surfaceTexture_in) { }

SurfaceShader::~SurfaceShader()
{
	
}