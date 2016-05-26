//
//  Scene.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/29/11.
//  Copyright 2011 Student. All rights reserved.
//

#pragma once

#include "Camera.h"
#include "Light.h"
#include "../Geometry/Shape.h"

#include <Math/color.h>

#include <memory>
#include <vector>

class EnvironmentMap;
class PhotonMap;

class Scene {
	public:
		Scene();
		~Scene();
	
		Camera* camera;
		int nPhotons;
		std::unique_ptr<PhotonMap> pMap;
		std::vector<Shape *> shapes;
		std::vector<Light *> lights;

		void addLight(Light *);

		EnvironmentMap* background;

		int nEstimatePhotons;
		float maxPhotonDist;
		bool usePMapDirect;

		float shutterTime;
};
