#include "msbase/mesh/get.h"
#include "ccglobal/log.h"

#include <limits.h>
#include <math.h>

namespace msbase
{
	trimesh::vec3 getFaceNormal(trimesh::TriMesh* mesh, int faceIndex)
	{
		trimesh::vec3 n = trimesh::vec3(0.0f, 0.0f, 0.0f);
		if (mesh && faceIndex >= 0 && faceIndex < (int)mesh->faces.size())
		{
			const trimesh::TriMesh::Face& f = mesh->faces.at(faceIndex);

			trimesh::vec3 v01 = mesh->vertices.at(f[1]) - mesh->vertices.at(f[0]);
			trimesh::vec3 v02 = mesh->vertices.at(f[2]) - mesh->vertices.at(f[0]);
			n = v01 TRICROSS v02;
			trimesh::normalize(n);
		}
		else
		{
			LOGW("msbase::getFaceNormal : error input.");
		}
		return n;
	}
}