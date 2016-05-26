/*! \file Mesh.h
    \author Wojciech Jarosz
*/

#pragma once

#include <Math/LimitsT.h>
#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Box.h>
#include <vector>
#include "stdint.h"

class Motion;

namespace Math {
	struct MeshBase{
		public:
			MeshBase() :
				vertices(0),
				normals(0),
				vertexColors(0),
				texCoords(0),
				materials(0),
				vertexIndices(0),
				materialIndices(0),
				numTris(0),
				numVertices(0),
				numMaterials(0),
				bbox() { }
    
			virtual ~MeshBase() { }
    
			void computeNormals();
			void flipNormals();
			void reverseOrientation();
			void calculateBBox();

			typedef Vec3<uint32_t> TupleI3;

			Vec3f* vertices;
			Vec3f* normals;
			Color3f* vertexColors;
			Vec2f* texCoords;
			std::string* materials;
			Motion* motion;
			unsigned int nTime;
			float shutterTime;

			TupleI3* vertexIndices;
			uint32_t* materialIndices;

			uint32_t numTris;
			uint32_t numVertices;
			uint32_t numMaterials;
    
			Box4f bbox;
	};
} // namespace Math
