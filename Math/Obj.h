/*! \file OBJ.h
    \author Wojciech Jarosz
*/
#ifndef MESH_OBJ_H
#define MESH_OBJ_H

#include <Math/MeshBase.h>
#include <string>

namespace Math
{

MeshBase * readObjMesh(const std::string& filename);

} // namespace Math

#endif // MESH_OBJ_H
