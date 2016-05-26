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
	
		std::unique_ptr<Camera> camera;

		std::vector<Shape*> shapes;
		std::vector<Light*> lights;
		EnvironmentMap* background;

		int nPhotons;
		int nEstimatePhotons;
		float maxPhotonDist;
		bool usePMapDirect;
		std::unique_ptr<PhotonMap> pMap;

		void addLight(Light*);
};
