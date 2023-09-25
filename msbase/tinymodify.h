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

	MSBASE_API trimesh::vec3 centerBox(const trimesh::vec3& center, const std::vector<trimesh::box3>& boxes, bool ignoreZ = true);

	MSBASE_API void reverseTriMesh(trimesh::TriMesh* Mesh);

	MSBASE_API trimesh::fxform layMatrixFromPositionNormal(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& scale, const trimesh::vec3& originNormal);
	MSBASE_API trimesh::fxform layArrowMatrix(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& scale);
}

#endif // MSBASE_TINYMODIFY_1695183239576_H