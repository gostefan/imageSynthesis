#include "Camera.h"
#include <Math/MathGL.h>

using namespace Math;

namespace {
	const double PI = 3.1415926f;
}

Camera::Camera(unsigned x, unsigned y) :
		m_resolution(x, y),
		m_fovy(30),
		m_aspect(x/float(y)),
		m_height(2.0f*tanf(radians(0.5f*m_fovy))),
		m_width(m_height*m_aspect) {
    m_worldToCamera.lookAt(Vec3f(0.0, 4.0f, 6.0f), Vec3f(0,3.5f,-10.0f), Vec3f(0.0f, 1.0f, 0.0f));
    //m_worldToCamera.lookAt(Vec3f(0.0, 2.0f, 3.0f), Vec3f(0,0.5,0.0f), Vec3f(0.0f, 1.0f, 0.0f));
	m_cameraToWorld = m_worldToCamera.inverse();
    m_perspective.perspective(m_fovy, m_aspect, 0.1f, 5000.0f);
	m_worldToNDC = m_perspective * m_worldToCamera;
    m_NDCToWindow.viewport(m_resolution.x, m_resolution.y);
	m_WindowToWorld = m_worldToNDC.inverse() * m_NDCToWindow.inverse();
}



void Camera::generateRay(Ray * r, float x, float y) const {
	// Assume x in 0:res_x - 1 and y in 0:res_y-1
	
	//the preallocated Ray r should have a world space origin and direction
	//and as well as a region of interest (using r->tMin and r->tMax)
	//given the image space coordinates x and y.

	r->o = this->m_cameraToWorld.colAsVec3(3);
	Math::Vec3f point = m_WindowToWorld * Math::Vec3f(x+0.5f, y+0.5f, 0);

	r->d = point - r->o;
	r->d.normalize();
}


void Camera::setResolution(unsigned x, unsigned y) {
	m_resolution.set(x, y);
	m_aspect = x / float(y);
	m_width = m_height*m_aspect;
	m_perspective.perspective(m_fovy, m_aspect, 0.1f, 1000.0f);
	m_worldToNDC = m_perspective * m_worldToCamera;
    m_NDCToWindow.viewport(m_resolution.x, m_resolution.y);
	m_WindowToWorld = m_worldToNDC.inverse() * m_NDCToWindow.inverse();
}

void Camera::setFOVY(float fov) {
	m_fovy = fov;
	m_height = 2.0f*tanf(radians(m_fovy)),
	m_width = m_height*m_aspect;
    m_perspective.perspective(m_fovy, m_aspect, 0.1f, 1000.0f);
	m_worldToNDC = m_perspective * m_worldToCamera;
	m_WindowToWorld = m_worldToNDC.inverse() * m_NDCToWindow.inverse();
}

void Camera::setWorldToCamera(const Mat44f & w2c) {
	m_worldToCamera = w2c;
	m_cameraToWorld = m_worldToCamera.inverse();
	
	m_worldToNDC = m_perspective * m_worldToCamera;
	m_WindowToWorld = m_worldToNDC.inverse() * m_NDCToWindow.inverse();
}

void Camera::setCameraToWorld(const Mat44f & c2w) {
	m_cameraToWorld = c2w;
	m_worldToCamera = m_cameraToWorld.inverse();

	m_worldToNDC = m_perspective * m_worldToCamera;
	m_WindowToWorld = m_worldToNDC.inverse() * m_NDCToWindow.inverse();
}

void Camera::renderGL() const {
    // Set perspective projection
    glMatrixMode(GL_PROJECTION);
	glLoadMatrix(m_perspective);
    
    // Place the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(m_worldToCamera);
}
