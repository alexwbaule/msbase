#ifndef MSBASE_PRIMITIVE_1695439721999_H
#define MSBASE_PRIMITIVE_1695439721999_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"

namespace msbase
{
	MSBASE_API trimesh::TriMesh* createSphere(float radius, int num_iter);

	MSBASE_API trimesh::TriMesh* createTorusMesh(float middler, float tuber, int sides, int slices);

	MSBASE_API trimesh::TriMesh* createCylinder(float radius, float height, int num_iter);

	MSBASE_API trimesh::TriMesh* createCCylinder(float radiusu, float radiusb, float height, int num_iter);

	MSBASE_API trimesh::TriMesh* createCuboid(float x, float y, float z);

	MSBASE_API trimesh::TriMesh* createCone(size_t num_iter, float radius, float height);
}

#endif // MSBASE_PRIMITIVE_1695439721999_H