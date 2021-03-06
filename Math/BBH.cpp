
#pragma once
/*! \file BBH.cpp
    \brief
    \author Wojciech Jarosz
*/

#include <Math/BBH.h>
#include <Math/MathGL.h>
#include <Platform/Progress.h>

#include <sstream>

namespace Math {

	BBH::BuildStats::BuildStats() :
			progress(nullptr) {
		numNodes = numLeaves = 0;
		sumObjects = 0;
		minObjects = Math::Limits<int>::max();
		maxObjects = Math::Limits<int>::min();
		sumDepth = 0;
		minDepth = Math::Limits<int>::max();
		maxDepth = Math::Limits<int>::min();
		numLeaves0 = 0;
		numLeaves1 = 0;
		numLeaves2 = 0;
		numLeaves3 = 0;
		numLeaves4 = 0;
		numLeaves4p = 0;
	}

	BBH::BuildStats::~BuildStats() { }; // Needs to be here for the destructor of the unique_ptr

	void BBH::BuildStats::startBuild(const char * title, int steps) {
		progress = std::unique_ptr<Platform::Progress>(new Platform::Progress(title, steps));
	}

	void BBH::BuildStats::stepProgress(int steps) {
		//(*progress) += steps;
	}

	void BBH::BuildStats::finishBuild() {
		progress->done();
	}

	void BBH::BuildStats::updateLeaf(int depth, int n) {
		numLeaves++;
		minDepth = std::min(depth, minDepth);
		maxDepth = std::max(depth, maxDepth);
		sumDepth += depth;
		minObjects = std::min(n, minObjects);
		maxObjects = std::max(n, maxObjects);
		sumObjects += n;
		switch (n) {
			case 0:
				numLeaves0++;
				break;
			case 1:
				numLeaves1++;
				break;
			case 2:
				numLeaves2++;
				break;
			case 3:
				numLeaves3++;
				break;
			case 4:
				numLeaves4++;
				break;
			default:
				numLeaves4p++;
				break;
		}
	}

	void BBH::BuildStats::printStats() const {
		std::stringstream out;
		out << std::setfill('=') << std::setw(78) << "" << std::endl;
		out << "BBH Construction Statistics:" << std::endl;
		out << std::setfill('=') << std::setw(78) << "" << std::endl;
	
		// print out your BVH statistics here for debugging

		out << "Nodes:" << numNodes << std::endl;
		out << "Leaves:" << numLeaves << std::endl;
    
		out << "   Objects in Leaves:" << std::endl;
		out << "   min:" << minObjects << std::endl;
		out << "   avg:" << ((float) sumObjects / numLeaves) << std::endl;
		out << "   avg(n>0):" << ((float) sumObjects / (numLeaves - numLeaves0)) << std::endl;    
		out << "   Leaf Depth:" << std::endl;
		out << "   min:" << minDepth << std::endl;
		out << "   avg:" << ((float) sumDepth / numLeaves) << std::endl;
		out << "   max:" << maxDepth << std::endl;

		out << std::setfill('=') << std::setw(78) << "" << std::endl;
    
		std::cout << out.str();
	}


	void BBH::Node::initInterior(BuildStats& stats, const Math::Box4f& box) {
		stats.updateInner();
		leaf = 0;
		bbox = box;
	}

	void BBH::Node::freeMemory() {
		if (isLeaf() && nObjects() > 1) {
			delete[] indices;
			nObjs = 0;
		}
	}

	void BBH::Node::renderGL() const {
		const Math::Vec4f e = bbox.max - bbox.min;
		glBegin(GL_LINE_LOOP);
			glVertex(bbox.min);
			glVertex(bbox.min + Math::Vec4f(e.x, 0, 0, 0));
			glVertex(bbox.min + Math::Vec4f(e.x, e.y, 0, 0));
			glVertex(bbox.min + Math::Vec4f(0.0f, e.y, 0, 0));
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex(bbox.min);
			glVertex(bbox.min + Math::Vec4f(0.0f, 0.0f, e.z, 0));
			glVertex(bbox.min + Math::Vec4f(0.0f, e.y, e.z, 0));
			glVertex(bbox.min + Math::Vec4f(0.0f, e.y, 0, 0));
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex(bbox.min);
			glVertex(bbox.min + Math::Vec4f(0.0f, 0.0f, e.z, 0));
			glVertex(bbox.min + Math::Vec4f(e.x, 0.0f, e.z, 0));
			glVertex(bbox.min + Math::Vec4f(e.x, 0.0f, 0, 0));
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex(bbox.max);
			glVertex(bbox.max - Math::Vec4f(e.x, 0.0f, 0, 0));
			glVertex(bbox.max - Math::Vec4f(e.x, e.y, 0, 0));
			glVertex(bbox.max - Math::Vec4f(0.0f, e.y, 0, 0));
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex(bbox.max);
			glVertex(bbox.max - Math::Vec4f(0.0f, 0.0f, e.z, 0));
			glVertex(bbox.max - Math::Vec4f(0.0f, e.y, e.z, 0));
			glVertex(bbox.max - Math::Vec4f(0.0f, e.y, 0, 0));
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex(bbox.max);
			glVertex(bbox.max - Math::Vec4f(0.0f, 0.0f, e.z, 0));
			glVertex(bbox.max - Math::Vec4f(e.x, 0.0f, e.z, 0));
			glVertex(bbox.max - Math::Vec4f(e.x, 0.0f, 0, 0));
		glEnd();
	}


	BBH::~BBH() {
		clear();
	}

	void BBH::clear() {
		for (auto& node : nodes)
			node.freeMemory();

		nodes.clear();
	}
} // namespace Math

