#ifndef MSBASE_GET_1695188680764_H
#define MSBASE_GET_1695188680764_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/quaternion.h"
#include "trimesh2/XForm.h"

namespace msbase
{
	//quaterian
	MSBASE_API trimesh::fxform fromQuaterian(const trimesh::quaternion& q);

	MSBASE_API trimesh::vec3 getFaceNormal(trimesh::TriMesh* mesh, int faceIndex);
}

#endif // MSBASE_GET_1695188680764_H