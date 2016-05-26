//
//  PhotonMappingRenderer.h
//  Image Synthesis Basecode
//
//  Created by Stefan Götschi on 04/30/12.
//  Copyright 2012 Student. All rights reserved.
//

#pragma once

#include <OGL/FBO.h>
#include <Math/Vec4.h>
#include <Math/Box.h>
#include "RayTracingRenderer.h"
#include <Platform/Progress.h>

class PhotonScaterer {
	public:
		PhotonScaterer(float continueProb = 0.5);
		~PhotonScaterer();
	
		virtual void scatter(Scene & scene);

	protected:
		void tracePhotons(Scene& scene);
		float continueProb;
};