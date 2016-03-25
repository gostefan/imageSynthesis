//
//  Scene.h
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/29/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef Rain_H
#define Rain_H

#include <Math/Vec3.h>
#include <Math/MeshBase.h>
#include <Math/BBH.h>

#include "Mesh.h"

class Sphere;

class Rain : public Mesh
{
protected:
	float radius;
	vector<Sphere*> drops;
	int nDrops;
	float getSpeed(float rad);
	
public:
	
	Rain(Box3f size, float scale, int nDrops, float shutterTime = 0, unsigned int nTime = 1,
		 int maxDepth = 64, int maxObjects = 1);
	
	void renderGL() const;
	
    bool intersect(Ray * r) const;
};

#endif // RENDER_APP_MESH_H