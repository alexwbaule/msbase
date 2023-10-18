#include "msbase/mesh/merge.h"
#include "msbase/mesh/dumplicate.h"
#include "msbase/utils/meshtopo.h"
#include <assert.h>

namespace msbase
{
	trimesh::TriMesh* mergeMeshes(const std::vector<std::shared_ptr<trimesh::TriMesh>>& ins)
	{
		std::vector<trimesh::TriMesh*> meshes;
		for (std::shared_ptr<trimesh::TriMesh> mesh : ins)
			meshes.push_back(mesh.get());

		return mergeMeshes(meshes);
	}

	trimesh::TriMesh* mergeMeshes(const std::vector<trimesh::TriMesh*>& inMeshes)
	{
		trimesh::TriMesh* outMesh = new trimesh::TriMesh();
		size_t totalVertexSize = outMesh->vertices.size();
		size_t totalUVSize = outMesh->cornerareas.size();
		size_t totalTriangleSize = outMesh->faces.size();

		size_t addVertexSize = 0;
		size_t addTriangleSize = 0;
		size_t addUVSize = 0;

		size_t meshSize = inMeshes.size();
		for (size_t i = 0; i < meshSize; ++i)
		{
			if (inMeshes.at(i))
			{
				addVertexSize += inMeshes.at(i)->vertices.size();
				addTriangleSize += inMeshes.at(i)->faces.size();
				addUVSize += inMeshes.at(i)->cornerareas.size();
			}
		}
		totalVertexSize += addVertexSize;
		totalTriangleSize += addTriangleSize;
		totalUVSize += addUVSize;

		if (addVertexSize > 0 && addTriangleSize > 0)
		{
			outMesh->vertices.reserve(totalVertexSize);
			outMesh->cornerareas.reserve(totalUVSize);
			outMesh->faces.reserve(totalTriangleSize);

			size_t startFaceIndex = outMesh->faces.size();
			size_t startVertexIndex = outMesh->vertices.size();;
			size_t startUVIndex = outMesh->cornerareas.size();
			for (size_t i = 0; i < meshSize; ++i)
			{
				trimesh::TriMesh* mesh = inMeshes.at(i);
				if (mesh)
				{
					int vertexNum = (int)mesh->vertices.size();
					int faceNum = (int)mesh->faces.size();
					int uvNum = (int)mesh->cornerareas.size();
					if (vertexNum > 0 && faceNum > 0)
					{
						outMesh->vertices.insert(outMesh->vertices.end(), mesh->vertices.begin(), mesh->vertices.end());
						outMesh->cornerareas.insert(outMesh->cornerareas.end(), mesh->cornerareas.begin(), mesh->cornerareas.end());
						outMesh->faces.insert(outMesh->faces.end(), mesh->faces.begin(), mesh->faces.end());

						size_t endFaceIndex = startFaceIndex + faceNum;
						if (startVertexIndex > 0)
						{
							for (size_t ii = startFaceIndex; ii < endFaceIndex; ++ii)
							{
								trimesh::TriMesh::Face& face = outMesh->faces.at(ii);
								for (int j = 0; j < 3; ++j)
									face[j] += startVertexIndex;
							}
						}

						startFaceIndex += faceNum;
						startVertexIndex += vertexNum;
						startUVIndex += uvNum;

					}
				}
			}
		}

		return outMesh;
	}

	trimesh::TriMesh* partMesh(const std::vector<int>& indices, trimesh::TriMesh* inMesh)
	{
		trimesh::TriMesh* outMesh = nullptr;
		if (inMesh && indices.size() > 0 && inMesh->vertices.size() > 0)
		{
			size_t vertexSize = inMesh->vertices.size();
			outMesh = new trimesh::TriMesh();
			outMesh->faces.resize(indices.size());
			outMesh->vertices.reserve(3 * indices.size());

			std::vector<int> flags(vertexSize, -1);
			int startIndex = 0;
			int k = 0;
			for (int i : indices)
			{
				trimesh::TriMesh::Face& f = inMesh->faces.at(i);
				trimesh::TriMesh::Face& of = outMesh->faces.at(k);
				for (int j = 0; j < 3; ++j)
				{
					int index = f[j];
					if (flags.at(index) < 0)
					{
						flags.at(index) = startIndex++;
						outMesh->vertices.push_back(inMesh->vertices.at(index));
					}

					of[j] = flags.at(index);
				}

				++k;
			}
		}

		return outMesh;
	}

	std::vector < std::vector<BaseMeshPtr>> meshSplit(const std::vector<BaseMeshPtr>& meshes, ccglobal::Tracer* progressor)
	{
		std::vector < std::vector<BaseMeshPtr>> outMeshes;
		outMeshes.reserve(meshes.size());

		for (auto mesh : meshes)
		{
			int originV = mesh->vertices.size();
			originV = originV >= 0 ? originV : 1;
			
			dumplicateMesh(mesh.get(), nullptr);;

			if (!mesh || mesh->faces.size() <= 0)
			{
				continue;
			}

			if (progressor)
			{
				progressor->progress(0.1f);
			}

			MeshTopo topo;
			topo.build(mesh.get());
			if (progressor)
			{
				progressor->progress(0.3f);

				if (progressor->interrupt())
				{
					return outMeshes;
				}
			}

			int faceNum = (int)mesh->faces.size();
			std::vector<bool> visitFlags(faceNum, false);

			std::vector<int> visitStack;
			std::vector<int> nextStack;

			std::vector<std::vector<int>> parts;
			parts.reserve(100);
			for (int faceID = 0; faceID < faceNum; ++faceID)
			{
				if (visitFlags.at(faceID) == false)
				{
					visitFlags.at(faceID) = true;
					visitStack.push_back(faceID);

					std::vector<int> facesChunk;
					facesChunk.push_back(faceID);
					while (!visitStack.empty())
					{
						int seedSize = (int)visitStack.size();
						for (int seedIndex = 0; seedIndex < seedSize; ++seedIndex)
						{
							int cFaceID = visitStack.at(seedIndex);
							trimesh::ivec3& oppoHalfs = topo.m_oppositeHalfEdges.at(cFaceID);
							for (int halfID = 0; halfID < 3; ++halfID)
							{
								int oppoHalf = oppoHalfs.at(halfID);
								if (oppoHalf >= 0)
								{
									int oppoFaceID = topo.faceid(oppoHalf);
									if (visitFlags.at(oppoFaceID) == false)
									{
										nextStack.push_back(oppoFaceID);
										facesChunk.push_back(oppoFaceID);
										visitFlags.at(oppoFaceID) = true;
									}
								}
							}
						}

						visitStack.swap(nextStack);
						nextStack.clear();
					}

					parts.push_back(std::vector<int>());
					parts.back().swap(facesChunk);
				}
				else
				{
					visitFlags.at(faceID) = true;
				}

				if ((faceID + 1) % 100 == 0)
				{
					if (progressor->interrupt())
					{
						return outMeshes;
					}
				}

			}

			std::vector<trimesh::TriMesh*> meshes;
			size_t partSize = parts.size();
			for (size_t i = 0; i < partSize; ++i)
			{
				if (parts.at(i).size() > 10)
				{
					trimesh::TriMesh* outMesh = partMesh(parts.at(i), mesh.get());
					if (outMesh) meshes.push_back(outMesh);
				}
			}

			//merge small ones
			int tSize = (int)meshes.size();
			int maxCount = 0;
			for (int i = 0; i < tSize; ++i)
			{
				if (maxCount < (int)meshes.at(i)->vertices.size())
					maxCount = (int)meshes.at(i)->vertices.size();
			}

			//int smallCount = (int)((float)maxCount * 0.05f);
			const int smallV = 150;
			const int samllF = 50;
			std::vector<trimesh::TriMesh*> allInOne;
			//std::vector<trimesh::TriMesh*> validMeshes;
			std::vector<BaseMeshPtr> validMeshes;
			for (int i = 0; i < tSize; ++i)
			{
				//if ((int)meshes.at(i)->vertices.size() < smallCount)
				float ratio = meshes.at(i)->vertices.size() * 1.0f / originV;
				if ((int)meshes.at(i)->vertices.size() < smallV
					&& (int)meshes.at(i)->faces.size() < samllF
					&& ratio < 0.1f)
					allInOne.push_back(meshes.at(i));
				else
					validMeshes.push_back(BaseMeshPtr(meshes.at(i)));
			}

			if (allInOne.size() > 0)
			{
				trimesh::TriMesh* newMesh = mergeMeshes(allInOne);
				validMeshes.push_back(BaseMeshPtr(newMesh));

				for (trimesh::TriMesh* m : allInOne)
					delete m;
				allInOne.clear();
			}
			outMeshes.push_back(validMeshes);
		}

		return outMeshes;
	}

	MSBASE_API trimesh::TriMesh* mergeColorMeshes(trimesh::TriMesh* sourceMesh, const std::vector<std::string>& color2Facets)
	{
		enum class EnforcerBlockerType : int8_t {
			// Maximum is 3. The value is serialized in TriangleSelector into 2 bits.
			NONE      = 0,
			ENFORCER  = 1,
			BLOCKER   = 2,
			// Maximum is 15. The value is serialized in TriangleSelector into 6 bits using a 2 bit prefix code.
			Extruder1 = ENFORCER,
			Extruder2 = BLOCKER,
			Extruder3,
			Extruder4,
			Extruder5,
			Extruder6,
			Extruder7,
			Extruder8,
			Extruder9,
			Extruder10,
			Extruder11,
			Extruder12,
			Extruder13,
			Extruder14,
			Extruder15,
			ExtruderMax
		};

		/* color2Facets字符串转二进制序列 */
		std::pair<std::vector<std::pair<int, int>>, std::vector<bool>> data;
		for (int i = 0, count = color2Facets.size(); i < count; ++i)
		{
			std::string str = color2Facets[i];
			if (str.empty())
				continue;

			data.first.emplace_back(i, int(data.second.size()));
			for (auto it = str.crbegin(); it != str.crend(); ++it) {
				const char ch = *it;
				int dec = 0;
				if (ch >= '0' && ch <= '9')
					dec = int(ch - '0');
				else if (ch >= 'A' && ch <= 'F')
					dec = 10 + int(ch - 'A');
				else
					assert(false);

				// Convert to binary and append into code.
				for (int i = 0; i < 4; ++i)
					data.second.insert(data.second.end(), bool(dec & (1 << i)));
			}
		}
        data.first.shrink_to_fit();
        data.second.shrink_to_fit();

		/* copy newMesh */
		trimesh::TriMesh* newMesh = new trimesh::TriMesh();
		int verticeCount = sourceMesh->vertices.size();
		newMesh->vertices.resize(verticeCount);
		for (int i = 0; i < verticeCount; ++i)
			newMesh->vertices[i]=sourceMesh->vertices[i];

		int faceCount = sourceMesh->faces.size();
		newMesh->faces.resize(faceCount);
		for (int i = 0; i < faceCount; ++i)
			newMesh->faces[i] = sourceMesh->faces[i];

		newMesh->flags.resize(sourceMesh->faces.size());

		/* deserialize */
		// Vector to store all parents that have offsprings.
		struct ProcessingInfo {
			int facet_id = 0;
			trimesh::vec3 neighbors { -1, -1, -1 };
			int processed_children = 0;
			int total_children = 0;
		};

		// Depth-first queue of a source mesh triangle and its childern.
		// kept outside of the loop to avoid re-allocating inside the loop.
		std::vector<ProcessingInfo> parents;

		for (std::pair<int, int> d : data.first) {
			int triangle_id = d.first, ibit = d.second;
			assert(triangle_id < int(newMesh->faces.size()));
			assert(ibit < int(data.second.size()));
			auto next_nibble = [&data, &ibit = ibit]() {
				int n = 0;
				for (int i = 0; i < 4; ++ i)
					n |= data.second[ibit ++] << i;
				return n;
			};

			parents.clear();
			while (true) {
				// Read next triangle info.
				int code = next_nibble();
				int num_of_split_sides = code & 0b11;
				int num_of_children = num_of_split_sides == 0 ? 0 : num_of_split_sides + 1;
				bool is_split = num_of_children != 0;
				// Only valid if not is_split. Value of the second nibble was subtracted by 3, so it is added back.
				auto state = is_split ? EnforcerBlockerType::NONE : EnforcerBlockerType((code & 0b1100) == 0b1100 ? next_nibble() + 3 : code >> 2);

				// BBS
				if (state > EnforcerBlockerType::ExtruderMax)
					state = EnforcerBlockerType::NONE;

				// Only valid if is_split.
				int special_side = code >> 2;

				// Take care of the first iteration separately, so handling of the others is simpler.
				if (parents.empty()) {
					// if (is_split) {
					// 	// root is split, add it into list of parents and split it.
					// 	// then go to the next.
					// 	Vec3i neighbors = m_neighbors[triangle_id];
					// 	parents.push_back({triangle_id, neighbors, 0, num_of_children});
					// 	m_triangles[triangle_id].set_division(num_of_split_sides, special_side);
					// 	perform_split(triangle_id, neighbors, EnforcerBlockerType::NONE);
					// 	continue;
					// } else {
						// root is not split. just set the state and that's it.
						newMesh->flags[triangle_id] = (int)state;
						break;
					// }
				}

				// // This is not the first iteration. This triangle is a child of last seen parent.
				// assert(! parents.empty());
				// assert(parents.back().processed_children < parents.back().total_children);

				// if (ProcessingInfo& last = parents.back();  is_split) {
				// 	// split the triangle and save it as parent of the next ones.
				// 	const Triangle &tr = m_triangles[last.facet_id];
				// 	int   child_idx = last.total_children - last.processed_children - 1;
				// 	Vec3i neighbors = this->child_neighbors(tr, last.neighbors, child_idx);
				// 	int this_idx = tr.children[child_idx];
				// 	m_triangles[this_idx].set_division(num_of_split_sides, special_side);
				// 	perform_split(this_idx, neighbors, EnforcerBlockerType::NONE);
				// 	parents.push_back({this_idx, neighbors, 0, num_of_children});
				// } else {
				// 	// this triangle belongs to last split one
				// 	int child_idx = last.total_children - last.processed_children - 1;
				// 	m_triangles[m_triangles[last.facet_id].children[child_idx]].set_state(state);
				// 	++last.processed_children;
				// }

				// // If all children of the past parent triangle are claimed, move to grandparent.
				// while (parents.back().processed_children == parents.back().total_children) {
				// 	parents.pop_back();

				// 	if (parents.empty())
				// 		break;

				// 	// And increment the grandparent children counter, because
				// 	// we have just finished that branch and got back here.
				// 	++parents.back().processed_children;
				// }

				// // In case we popped back the root, we should be done.
				// if (parents.empty())
				// 	break;
			}
		}
		return newMesh;
	}
}