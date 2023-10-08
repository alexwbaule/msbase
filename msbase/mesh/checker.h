#ifndef MSBASE_CHECKER_1691396111581_H
#define MSBASE_CHECKER_1691396111581_H
#include "msbase/interface.h"
#include "trimesh2/TriMesh.h"

namespace msbase
{
	MSBASE_API bool checkDegenerateFace(trimesh::TriMesh* mesh, bool remove = false);
}

#endif // MSBASE_CHECKER_1691396111581_H