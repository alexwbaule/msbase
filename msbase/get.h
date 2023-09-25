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

	inline trimesh::dvec3 vec32dvec3(const trimesh::vec3& v)
	{
		return trimesh::dvec3(v.x, v.y, v.z);
	}

	MSBASE_API float angleOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);
	MSBASE_API float radianOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2);
}

#endif // MSBASE_GET_1695188680764_H