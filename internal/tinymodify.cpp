#include "msbase/tinymodify.h"
#include "trimesh2/TriMesh_algo.h"

namespace msbase
{
	trimesh::vec3 moveTrimesh2Center(trimesh::TriMesh* mesh, bool zZero)
	{
		if (!mesh)
			return trimesh::vec3(0.0f, 0.0f, 0.0f);

		mesh->need_bbox();
		trimesh::box3 b = mesh->bbox;

		trimesh::vec3 size = b.size() / 2.0f;
		size.x = 0.0f;
		size.y = 0.0f;
		if (!zZero)
			size.z = 0.0f;

		trimesh::vec3 offset = size - b.center();
		trimesh::trans(mesh, offset);

		return offset;
	}

	void offsetPoints(std::vector<trimesh::vec3>& points, const trimesh::vec3& offset)
	{
		for (trimesh::vec3& point : points)
			point += offset;
	}

	void applyMatrix2Points(std::vector<trimesh::vec3>& points, const trimesh::fxform& xf)
	{
		for (trimesh::vec3& point : points)
			point = xf * point;
	}

	void reverseTriMesh(trimesh::TriMesh* Mesh)
	{
		for (size_t i = 0; i < Mesh->faces.size(); i++)
		{
			int temp = Mesh->faces[i].at(1);
			Mesh->faces[i].at(1) = Mesh->faces[i].at(2);
			Mesh->faces[i].at(2) = temp;
		}
	}
}