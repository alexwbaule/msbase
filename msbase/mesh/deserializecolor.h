#ifndef MSBASE_DESERIALIZECOLOR_1695441390758_H
#define MSBASE_DESERIALIZECOLOR_1695441390758_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"
#include "ccglobal/tracer.h"

namespace msbase
{
	MSBASE_API trimesh::TriMesh* mergeColorMeshes(trimesh::TriMesh* sourceMesh, const std::vector<std::string>& color2Facets, ccglobal::Tracer* tracer = nullptr);
}

#endif // MSBASE_DESERIALIZECOLOR_1695441390758_H