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

    bool generateChunk(
        trimesh::TriMesh* mesh
        , const std::vector<trimesh::ivec3>& neigbs
        , const float max_area_per_chunk
        , std::vector<int>& m_faceChunkIDs
        , std::vector<std::vector<int>>& m_chunkFaces)
    {
        if (!mesh || mesh->faces.size() == 0)
        {
            LOGW("msbase::getFaceNormal : error input.");
            return false;
        }

        std::vector<trimesh::vec3> normals;
        std::vector<float> areas;
        calculateFaceNormalOrAreas(mesh,normals,&areas);

        //split chunks
        int faceCount = mesh->faces.size();
        //assert(faceCount == (int)neigbs.size());
        if (faceCount != (int)neigbs.size())
        {
            return false;
        }

        int chunkCount = 50;
        //if (faceCount < chunkCount)
        //    chunkCount = faceCount;

        int max_chunk_count = (int)faceCount / chunkCount;

        int currentChunk = 0;
        m_chunkFaces.reserve(chunkCount);
        m_faceChunkIDs.resize(faceCount, -1);

        m_chunkFaces.push_back(std::vector<int>());
        float area = 0.0f;
        int chunkSize = (int)faceCount / chunkCount / 2;
        for (int i = 0; i < faceCount; ++i)
        {
            if (m_faceChunkIDs.at(i) >= 0)
                continue;

            std::set<int> seeds;
            seeds.insert(i);
            std::set<int> next_seeds;

            while (!seeds.empty())
            {
                std::vector<int>& chunks = m_chunkFaces.at(currentChunk);

                for (int s : seeds)
                {
                    const trimesh::ivec3& nei = neigbs.at(s);
                    chunks.push_back(s);

                    area += areas[s];

                    m_faceChunkIDs.at(s) = currentChunk;

                    for (int j = 0; j < 3; ++j)
                    {
                        if (nei[j] >= 0 && m_faceChunkIDs.at(nei[j]) < 0)
                            next_seeds.insert(nei[j]);
                    }
                }

                if ((currentChunk < chunkCount - 1) && (/*next_seeds.empty() ||*/ (area > max_area_per_chunk && chunks.size() >= chunkSize)))
                {
                    currentChunk++;
                    m_chunkFaces.push_back(std::vector<int>());
                    next_seeds.clear();
                    area = 0.0f;
                }

                next_seeds.swap(seeds);
                next_seeds.clear();
            }
        }
        return true;
    }
}