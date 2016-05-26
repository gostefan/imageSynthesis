/*! \file Camera.h
 \brief Contains the Camera class
 \author Wojciech Jarosz
*/

#pragma once

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Mat44.h>
#include "Ray.h"


/*!
	This class represents a camera. The camera handles mapping
	camera space to world space (and visa-versa).
*/
class Camera {
	protected:
		Math::Vec2i m_resolution;
		Math::Mat44f m_worldToCamera, m_cameraToWorld,
					 m_perspective,
					 m_worldToNDC, m_NDCToWindow,
					 m_WindowToWorld;
		float m_fovy;
		float m_aspect;
		float m_height, m_width;

	public:
		Camera(unsigned resX = 640, unsigned resY = 480);
	
		virtual void renderGL() const;
		virtual void generateRay(Ray * r, float x, float y) const;
	
		virtual unsigned xRes() const {return m_resolution.x;}
		virtual unsigned yRes() const {return m_resolution.y;}
	
		virtual void setResolution(unsigned resX, unsigned resY);
	
		virtual float FOVY() const {return m_fovy;}
		virtual void setFOVY(float fovy);
	
		virtual const Math::Mat44f worldToCamera() const {return m_worldToCamera;}
		virtual const Math::Mat44f cameraToWorld() const {return m_cameraToWorld;}
		virtual const Math::Mat44f worldToNDC() const {return m_worldToNDC;}
		virtual const Math::Mat44f NDCToWindow() const {return m_NDCToWindow;}
	
		virtual void setWorldToCamera(const Math::Mat44f & w2c);
		virtual void setCameraToWorld(const Math::Mat44f & c2w);
};