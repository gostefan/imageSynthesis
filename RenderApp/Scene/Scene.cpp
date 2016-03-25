#include "Scene.h"
#include "photonmap.h"
#include "Camera.h"
#include "EnvironmentMap.h"

Scene::Scene() : nPhotons(0), usePMapDirect(false), nEstimatePhotons(100), maxPhotonDist(0.1f) {
	pMap = new Photon_map(0);
	camera = new Camera();
	background = new EnvironmentMap("./imgs/city1.hdr");
}

Scene::~Scene() {
	delete pMap;
	delete camera;
	delete background;
}

void Scene::addLight(Light* light) {
	this->lights.push_back(light);
	this->shapes.push_back(light);
}

