#ifndef MSBASE_MERGE_1695441390758_H
#define MSBASE_MERGE_1695441390758_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"

namespace msbase
{
	MSBASE_API trimesh::TriMesh* mergeMeshes(const std::vector<trimesh::TriMesh*>& ins);
}

#endif // MSBASE_MERGE_1695441390758_H