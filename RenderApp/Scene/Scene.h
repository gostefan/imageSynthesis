//
//  Scene.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/29/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include <Math/color.h>
#include <vector>
#include "../Geometry/Shape.h"
#include "Camera.h"
#include "Light.h"

class EnvironmentMap;
class Photon_map;

class Scene
{
public:
	Scene();

	~Scene();
	
	Camera* camera;
	int nPhotons;
	Photon_map* pMap;
	std::vector<Shape *> shapes;
	std::vector<Light *> lights;

	void addLight(Light *);

	EnvironmentMap* background;

	int nEstimatePhotons;
	float maxPhotonDist;
	bool usePMapDirect;

	float shutterTime;
};


#endif
