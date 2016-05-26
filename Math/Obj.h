/*! \file OBJ.h
    \author Wojciech Jarosz
*/

#pragma once

#include <Math/MeshBase.h>
#include <string>

namespace Math {
	MeshBase* readObjMesh(const std::string& filename);
} // namespace Math
