
#include "Rain.h"

#include <math.h>
#include <Math/LineAlgo.h>
#include "Motion.h"
#include "../Shader/RefractionShader.h"
#include "Sphere.h"
#include "Proc.h"

namespace {
	double round(double d) {
		return floor(d + 0.5);
	}
}
	
Rain::Rain(Box3f size, float scale, int nDrops, float shutterTime, unsigned int nTime, int maxDepth, int maxObjects) :
		Mesh(0, 0, 0, shutterTime, nTime, maxDepth, maxObjects), nDrops(nDrops) {
	surfaceShader = new RefractionShader(1.333f, Color3f(0.1f, 0.1f, 0.8f), 0.8f);

	drops = vector<Sphere*>(nDrops);
	for (int i = 0; i < nDrops; i++) {
		float x = (static_cast<float>(rand()) / RAND_MAX) * (size.max[0] - size.min[0]) + size.min[0];
		float y = (static_cast<float>(rand()) / RAND_MAX) * (size.max[1] - size.min[1]) + size.min[1];
		float z = (static_cast<float>(rand()) / RAND_MAX) * (size.max[2] - size.min[2]) + size.min[2];
		float currRad = (static_cast<float>(rand()) / RAND_MAX) * 3; // Here it is in m but I devide later by 1000
		Motion* currMotion = new Motion(Vec3f(0, -getSpeed(currRad) * scale, 0));
		drops[i] = new Sphere(surfaceShader, Vec3f(x, y, z), currRad * scale / 1000, currMotion);
	}

	Proc proc(drops, motion, nTime, m_maxDepth, m_maxObjects, shutterTime);
	
	BBH::BuildStats stats;
	m_bbh.buildTree(proc, stats);
	stats.printStats();
}
	
void Rain::renderGL() const {
	for (unsigned i = 0; i < drops.size(); i++)
		drops[i]->renderGL();
}
	
bool Rain::intersect(Ray * r) const {
    const BBH::Node * todo[64];
    int todoPos = 0;
	
    int hitIndex = -1;
    const BBH::Node *node = &m_bbh.nodes[0];
    while (node) {
        if (node->isLeaf()) {
            // Check for intersections inside leaf node
            unsigned nObjects = node->nObjects();
            if (nObjects == 1) {
                const int index = node->oneIndex;
				drops[index]->intersect(r);
            } else if (nObjects > 1) {
                const unsigned * indices = node->indices;
                for (unsigned i = 0; i < nObjects; ++i) {
                    const unsigned index = indices[i];
					drops[index]->intersect(r);
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

float Rain::getSpeed(float rad) {
	// Values are from http://www.madsci.org/posts/archives/2000-07/962626446.Ph.r.html
	float rounded = round(rad*10)*0.1f;
	if (rounded <= 0.1)
		return 0.27f;
	else if (rounded <= 0.2f)
		return 0.72f;
	else if (rounded <= 0.3f)
		return 1.17f;
	else if (rounded <= 0.4f)
		return 1.62f;
	else if (rounded <= 0.5f)
		return 2.06f;
	else if (rounded <= 0.6f)
		return 2.47f;
	else if (rounded <= 0.7f)
		return 2.87f;
	else if (rounded <= 0.8f)
		return 3.27f;
	else if (rounded <= 0.9f)
		return 3.67f;
	else if (rounded <= 1)
		return 4.03f;
	else if (rounded <= 1.2f)
		return 4.64f;
	else if (rounded <= 1.4f)
		return 5.17f;
	else if (rounded <= 1.6f)
		return 5.65f;
	else if (rounded <= 1.8f)
		return 6.09f;
	else if (rounded <= 2)
		return 6.49f;
	else if (rounded <= 2.2f)
		return 6.90f;
	else if (rounded <= 2.4f)
		return 7.27f;
	else if (rounded <= 2.6f)
		return 7.57f;
	else if (rounded <= 2.8f)
		return 7.82f;
	else if (rounded <= 3)
		return 8.06f;
	else if (rounded <= 3.2f)
		return 8.26f;
	else if (rounded <= 3.4f)
		return 8.44f;
	else if (rounded <= 3.6f)
		return 8.60f;
	else if (rounded <= 3.8f)
		return 8.72f;
	else if (rounded <= 4)
		return 8.83f;
	else if (rounded <= 4.2f)
		return 8.92f;
	else if (rounded <= 4.4f)
		return 8.98f;
	else if (rounded <= 4.6f)
		return 9.03f;
	else if (rounded <= 4.8f)
		return 9.07f;
	else if (rounded <= 5)
		return 9.09f;
	else if (rounded <= 5.2f)
		return 9.12f;
	else if (rounded <= 5.4f)
		return 9.14f;
	else if (rounded <= 5.6f)
		return 9.16f;
	else //if (rounded <= 5.8f)
		return 9.17f;
}