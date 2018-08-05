#include "Scene.h"
#include "photonmap.h"
#include "Camera.h"
#include "EnvironmentMap.h"

Scene::Scene() : camera(new Camera()), nPhotons(0), usePMapDirect(false), nEstimatePhotons(100), maxPhotonDist(0.1f) {
	pMap.reset(new PhotonMap(0));
	//background = new EnvironmentMap("./imgs/city1.hdr");
}

Scene::~Scene() {
}

void Scene::addLight(Light* light) {
	this->lights.push_back(light);
	this->shapes.push_back(light);
}

