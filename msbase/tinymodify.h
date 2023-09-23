#ifndef MSBASE_TINYMODIFY_1695183239576_H
#define MSBASE_TINYMODIFY_1695183239576_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"

namespace msbase
{
	MSBASE_API trimesh::vec3 moveTrimesh2Center(trimesh::TriMesh* mesh, bool zZero = true);

	MSBASE_API void offsetPoints(std::vector<trimesh::vec3>& points, const trimesh::vec3& offset);
	MSBASE_API void applyMatrix2Points(std::vector<trimesh::vec3>& points, const trimesh::fxform& xf);

	MSBASE_API void reverseTriMesh(trimesh::TriMesh* Mesh);
}

#endif // MSBASE_TINYMODIFY_1695183239576_H