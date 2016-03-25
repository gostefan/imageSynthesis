//
//  Scene.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/29/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef RENDER_APP_MESH_H
#define RENDER_APP_MESH_H

#include "Shape.h"

#include <Math/Vec3.h>
#include <Math/MeshBase.h>
#include <Math/BBH.h>

class Mesh : public Shape
{
protected:
	Math::MeshBase * m_mesh;
	Math::BBH m_bbh;
    int m_maxDepth;
    int m_maxObjects;
	//SurfaceShader * m_shader;
	int nTime;

	void getLocalCoordinates(int index, HitInfo& hit);
	
public:
	
	Mesh(SurfaceShader * surfaceShader,
		 Math::MeshBase * mesh, Motion* motion = 0, float shutterTime = 0, unsigned int nTime = 1,
		 int maxDepth = 64, int maxObjects = 1);
	
	void renderGL() const;
	
    bool intersect(Ray * r) const;
    void fillHitInfo(Ray * r) const;
	
    Math::Vec3f evalP(float u, float v) const;
    Math::Vec3f evalN(float u, float v) const;
	virtual BBox getBBox() const;
	virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const;
};

#endif // RENDER_APP_MESH_H