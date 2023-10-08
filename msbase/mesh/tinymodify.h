#ifndef MSBASE_TINYMODIFY_1695183239576_H
#define MSBASE_TINYMODIFY_1695183239576_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"

namespace msbase
{
	MSBASE_API trimesh::vec3 moveTrimesh2Center(trimesh::TriMesh* mesh, bool zZero = true);

	MSBASE_API void reverseTriMesh(trimesh::TriMesh* Mesh);
	MSBASE_API void fillTriangleSoupFaceIndex(trimesh::TriMesh* mesh);
}

#endif // MSBASE_TINYMODIFY_1695183239576_H