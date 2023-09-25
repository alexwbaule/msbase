#include "msbase/tinymodify.h"
#include "trimesh2/TriMesh_algo.h"

#include "msbase/get.h"

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

	trimesh::vec3 centerBox(const trimesh::vec3& center, const std::vector<trimesh::box3>& boxes, bool ignoreZ)
	{
		trimesh::box3 allBox;
		for (const trimesh::box3& abox : boxes)
		{
			allBox += abox;
		}
		trimesh::vec3 offset = center - allBox.center();
		if (ignoreZ)
		{
			offset.z = 0.0;
		}
		return offset;
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

	trimesh::fxform layMatrixFromPositionNormal(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& scale, const trimesh::vec3& originNormal)
	{
		trimesh::fxform matrix = trimesh::fxform::identity();

		float angle = radianOfVector3D2(normal, originNormal);
		trimesh::vec3 axis = trimesh::cross(originNormal, normal);
		trimesh::normalize(axis);

		trimesh::fxform scaleMatrix = trimesh::fxform::scale(scale.x, scale.y, scale.z);
		trimesh::fxform transMatrix = trimesh::fxform::trans(position);
		if (abs(angle) < 0.001)
		{
			matrix = transMatrix * scaleMatrix;
		}
		else {
			trimesh::fxform rotateMatrix = trimesh::fxform::rot(angle, axis);
			matrix = transMatrix * rotateMatrix * scaleMatrix;
		}

		return matrix;
	}


	trimesh::fxform layArrowMatrix(const trimesh::vec3& position, const trimesh::vec3& normal, const trimesh::vec3& scale)
	{
		trimesh::vec3 originNormal(0.0f, 0.0f, 1.0f);
		return layMatrixFromPositionNormal(position, normal, scale, originNormal);
	}
}