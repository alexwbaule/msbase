#ifndef MSBASE_GET_1695188680764_H
#define MSBASE_GET_1695188680764_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"

namespace msbase
{
	MSBASE_API trimesh::vec3 getFaceNormal(trimesh::TriMesh* mesh, int faceIndex);
}

#endif // MSBASE_GET_1695188680764_H