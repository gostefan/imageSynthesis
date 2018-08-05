/*! \file OBJ.h
    \author Wojciech Jarosz
*/

#pragma once

#include <Math/MeshBase.h>
#include <string>

namespace Math {
	MeshBasePtr readObjMesh(const std::string& filename);
} // namespace Math
