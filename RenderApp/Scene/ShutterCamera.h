
#include "Camera.h"

class ShutterCamera : public Camera {
	public:
		ShutterCamera(float shutterTime, Camera* recursiveCamera);
		virtual void generateRay(Ray* r, float x, float y) const;

		virtual void renderGL() const { recursiveCamera->renderGL(); }
		virtual unsigned xRes() const { return recursiveCamera->xRes(); }
		virtual unsigned yRes() const { return recursiveCamera->yRes(); }
	
		virtual void setResolution(unsigned resX, unsigned resY) { recursiveCamera->setResolution(resX, resY); }
	
		virtual float FOVY() const { return recursiveCamera->FOVY(); }
		virtual void setFOVY(float fovy) { recursiveCamera->setFOVY(fovy); }
	
		virtual const Math::Mat44f worldToCamera() const { return recursiveCamera->worldToCamera(); }
		virtual const Math::Mat44f cameraToWorld() const { return recursiveCamera->cameraToWorld(); }
		virtual const Math::Mat44f worldToNDC() const { return recursiveCamera->worldToNDC(); }
		virtual const Math::Mat44f NDCToWindow() const { return recursiveCamera->NDCToWindow(); }
	
		virtual void setWorldToCamera(const Math::Mat44f& w2c) { recursiveCamera->setWorldToCamera(w2c); }
		virtual void setCameraToWorld(const Math::Mat44f& c2w) { recursiveCamera->setCameraToWorld(c2w); }
	protected:
		float shutterTime;
		Camera* recursiveCamera;
};