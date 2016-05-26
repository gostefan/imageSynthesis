//
//  Mesh.cpp
//  ImageSynthesisFramework
//
//  Created by wjarosz on 3/13/12.
//  Copyright (c) 2012. All rights reserved.
//

#include "Mesh.h"
#include <iostream>
#include <Math/MeshBase.h>
#include <Math/MathGL.h>
#include <Math/Core.h>
#include <Math/LineAlgo.h>
#include "Motion.h"
#include "Sphere.h"
#include "Proc.h"

using namespace Math;
using std::vector;

namespace {
	bool intersectTriangle(Ray* r, MeshBase* mesh, unsigned int index) {
		Vec3f transl = Vec3f(0,0,0);
		if (mesh->motion != nullptr)
			transl = mesh->motion->getTranslation(r->time);
		if (Math::intersect(r->o, r->d,
						   mesh->vertices[mesh->vertexIndices[index].x]+transl,
						   mesh->vertices[mesh->vertexIndices[index].y]+transl,
						   mesh->vertices[mesh->vertexIndices[index].z]+transl,
						   r->tMin, r->tMax,
						   &r->hit.t, &r->hit.uv, &r->hit.Ng))
		{
			r->hit.Ng.normalize();
			//r->hit.N = r->hit.Ng.normalize(); Interpolate normal barycentrically
			if (mesh->normals != nullptr) {
				Vec3f normal = mesh->normals[mesh->vertexIndices[index].x] * (1 - r->hit.uv.x - r->hit.uv.y);
				normal += mesh->normals[mesh->vertexIndices[index].y] * r->hit.uv.x;
				normal += mesh->normals[mesh->vertexIndices[index].z] * r->hit.uv.y;
				r->hit.N = normal.normalized();
			} else 
				r->hit.N = r->hit.Ng;

			r->hit.P = r->o + r->hit.t * r->d;
			if (mesh->texCoords != nullptr) {
				Vec2f texCoord = mesh->texCoords[mesh->vertexIndices[index].x] * (1 - r->hit.uv.x - r->hit.uv.y);
				texCoord += mesh->texCoords[mesh->vertexIndices[index].y] * r->hit.uv.x;
				texCoord += mesh->texCoords[mesh->vertexIndices[index].z] * r->hit.uv.y;
				r->hit.uv = texCoord;
			}
			return true;
		}
		return false;
	}
} // namespace 




Mesh::Mesh(SurfaceShader* ss, 
		Math::MeshBase* mesh, Motion* motion, float shutterTime, unsigned int nTime, int maxDepth, int maxObjects) :
		Shape(ss),
		m_mesh(mesh),
		m_maxDepth(maxDepth),
		m_maxObjects(maxObjects)
{
	if (mesh != nullptr) {
		mesh->motion = motion;
		mesh->nTime = nTime;
		mesh->shutterTime = shutterTime;
		Proc proc(m_mesh, m_maxDepth, m_maxObjects, shutterTime);
		
		BBH::BuildStats stats;
		m_bbh.buildTree(proc, stats);
		stats.printStats();
	}
}

void Mesh::renderGL() const {
	if (!m_mesh)
		return;

	glColor3f(1.0f, 1.0f, 1.0f);
	/*glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_mesh->numTris; ++i)
	{
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].x]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].y]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].z]);
	}*/
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < m_mesh->numTris; ++i) {
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].x]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].y]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].y]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].z]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].z]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].x]);
	}
	glEnd();
}

bool Mesh::intersect(Ray* r) const {
	#define MAX_TODO 64
    const BBH::Node * todo[MAX_TODO];
    int todoPos = 0;
	
    int hitIndex = -1;
    const BBH::Node *node = &m_bbh.nodes[0];
    while (node) {
        if (node->isLeaf()) {
            // Check for intersections inside leaf node
            unsigned nObjects = node->nObjects();
            if (nObjects == 1) {
                const int index = node->oneIndex;
                if (intersectTriangle(r, m_mesh, index)) {
					r->hit.shape = this;
					r->hit.surfaceShader = this->surfaceShader;
					r->hit.I = r->d;
                    r->tMax = r->hit.t;
					r->hit.time = r->time;
                    hitIndex = index;
                }
            } else if (nObjects > 1) {
                const unsigned * indices = node->indices;
                for (unsigned i = 0; i < nObjects; ++i) {
                    const unsigned index = indices[i];
                    if (intersectTriangle(r, m_mesh, index)) {
						r->hit.shape = this;
						r->hit.surfaceShader = this->surfaceShader;
						r->hit.I = r->d;
                        r->tMax = r->hit.t;
						r->hit.time = r->time;
                        hitIndex = index;
                    }
                }
            }
        } else {
            if (Math::intersects(r->o, r->d, r->time, node->bbox, r->tMin, r->tMax)) {
                // Enqueue secondChild in todo list
                todo[todoPos] = &m_bbh.nodes[node->right];
                ++todoPos;
				
                // Advance to next child node
                ++node;
                continue;
            }
        }
		
        // Grab next node to process from todo list
        if (todoPos > 0) {
            --todoPos;
            node = todo[todoPos];
        } else
            break;
    }
	
    return (hitIndex != -1 && r->hit.shape);
}


void Mesh::fillHitInfo(Ray* r) const {
	r->hit.surfaceShader = surfaceShader;
}

Vec3f Mesh::evalP(float u, float v) const {
	// Reyes rendering not supported yet for triangle meshes
	return Vec3f(0.0f);
}

Vec3f Mesh::evalN(float u, float v) const {
	// Reyes rendering not supported yet for triangle meshes
	return Vec3f(0.0f);
}


BBox Mesh::getBBox() const {
	return BBox(Vec3f(0,0,0), Vec3f(0,0,0));
}
BBox Mesh::getBBox(float uStart, float uEnd, float vStart, float vEnd) const {
	return BBox(Vec3f(0,0,0), Vec3f(0,0,0));
}

void Mesh::getLocalCoordinates(int index, HitInfo& hit) {
	// Set up equation system
	// a * u_1-0 + b * u_2-0 = (1||0)
	// a * v_1-0 + b * v_2-0 = [0||1)

	// [ u_1-0 u_2-0 ]   [ a ] = (1||0)
	// [ v_1-0 v_2-0 ] * [ b ] = (0||1)

	Vec3f p0 = m_mesh->vertices[m_mesh->vertexIndices[index].x];
	Vec3f p1 = m_mesh->vertices[m_mesh->vertexIndices[index].y];
	Vec3f p2 = m_mesh->vertices[m_mesh->vertexIndices[index].z];
	Vec2f t0 = m_mesh->texCoords[m_mesh->vertexIndices[index].x];
	Vec2f t1 = m_mesh->texCoords[m_mesh->vertexIndices[index].y];
	Vec2f t2 = m_mesh->texCoords[m_mesh->vertexIndices[index].z];
	Vec2f diff1 = t1 - t0;
	Vec2f diff2 = t2 - t0;
	
	Mat44f system = Mat44f(t1.x - t0.x, t2.x - t0.x, 0, 0, t1.y - t0.y, t2.y - t0.y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	Mat44f inverse = system.inverse();
	Vec4f oneZero = Vec4f(1+t0.x, t0.y, 0, 1);
	Vec4f zeroOne = Vec4f(t0.x,1+t0.y, 0, 1);

	Vec4f ab1 = inverse * oneZero;
	Vec4f ab2 = inverse * zeroOne;

	Mat44f differences = Mat44f(p1.x - p0.x, p2.x - p0.x, 0, 0, p1.y - p0.y, p2.y - p0.y, 0, 0, p1.z - p0.z, p2.z - p0.z, 0, 0, 0, 0, 0, 1);
	Vec4f sol1 = differences * ab1;
	Vec4f sol2 = differences * ab2;
	hit.dPdu = Vec3f(sol1.x, sol1.y, sol1.z);
	hit.dPdv = Vec3f(sol2.x, sol2.y, sol2.z);
};