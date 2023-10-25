#include "msbase/mesh/get.h"
#include "ccglobal/log.h"

#include <limits.h>
#include <math.h>
#include <set>

namespace msbase
{
	trimesh::vec3 getFaceNormal(trimesh::TriMesh* mesh, int faceIndex, bool normalized)
	{
		trimesh::vec3 n = trimesh::vec3(0.0f, 0.0f, 0.0f);
		if (mesh && faceIndex >= 0 && faceIndex < (int)mesh->faces.size())
		{
			const trimesh::TriMesh::Face& f = mesh->faces.at(faceIndex);

			trimesh::vec3 v01 = mesh->vertices.at(f[1]) - mesh->vertices.at(f[0]);
			trimesh::vec3 v02 = mesh->vertices.at(f[2]) - mesh->vertices.at(f[0]);
			n = v01 TRICROSS v02;
			if(normalized)
				trimesh::normalize(n);
		}
		else
		{
			LOGW("msbase::getFaceNormal : error input.");
		}
		return n;
	}

	void calculateFaceNormalOrAreas(trimesh::TriMesh* mesh, 
		std::vector<trimesh::vec3>& normals, std::vector<float>* areas)
	{
		if (!mesh || mesh->faces.size() == 0)
		{
			LOGW("msbase::getFaceNormal : error input.");
			return;
		}

		const int faceNum = mesh->faces.size();
		normals.resize(faceNum);

		for (int i = 0; i < faceNum; ++i) {
			normals.at(i) = getFaceNormal(mesh, i, false);
		}

		//std::transform
		if (areas) {
			areas->resize(faceNum, 0.0f);
			for (int i = 0; i < faceNum; ++i) {
				areas->at(i) = (trimesh::len(normals.at(i)) / 2.0);
			}
		}

		for (auto& n : normals) {
			trimesh::normalize(n);
		}
	}
}