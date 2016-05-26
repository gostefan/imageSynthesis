/*! \file BicubicPatch.h
 \brief Contains the BicubicPatch class
 \author Wojciech Jarosz
 */

#pragma once

#include "Shape.h"
#include "BBox.h"
#include <Math/Vec3.h>

using namespace std;

class BicubicPatch : public Shape {
	private:
		Math::Vec3f controlVertices[4][4];

	public:
	
		BicubicPatch(SurfaceShader* ss = nullptr);
		BicubicPatch(SurfaceShader* ss, const Math::Vec3f cvs[4][4], Displacement* displacement = nullptr);
	
		void renderGL() const;
	
		Math::Vec3f evalP(float u, float v) const;
		Math::Vec3f evalN(float u, float v) const;
	
		virtual BBox getBBox() const;
		virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const;
};
