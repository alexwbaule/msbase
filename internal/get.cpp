#include "msbase/get.h"
#include "ccglobal/log.h"

#include <limits.h>
#include <math.h>

namespace msbase
{
	trimesh::fxform fromQuaterian(const trimesh::quaternion& q)
	{
		float x2 = q.xp * q.xp;
		float y2 = q.yp * q.yp;
		float z2 = q.zp * q.zp;
		float xy = q.xp * q.yp;
		float xz = q.xp * q.zp;
		float yz = q.yp * q.zp;
		float wx = q.wp * q.xp;
		float wy = q.wp * q.yp;
		float wz = q.wp * q.zp;


		// This calculation would be a lot more complicated for non-unit length quaternions
		// Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
		//   OpenGL
		trimesh::fxform m = trimesh::fxform(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		trimesh::transpose(m);
		return m;
	}

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

	float angleOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2)
	{
		float radian = radianOfVector3D2(v1, v2);
		return radian * 180.0f / (float)M_PI;
	}

	float radianOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2)
	{
		double denominator = sqrt((double)trimesh::len2(v1) * (double)trimesh::len2(v2));
		double cosinus = 0.0;

		if (denominator < INT_MIN)
			cosinus = 1.0; // cos (1)  = 0 degrees

		cosinus = (double)(v1 DOT v2) / denominator;
		cosinus = cosinus > 1.0 ? 1.0 : (cosinus < -1.0 ? -1.0 : cosinus);
		double angle = acos(cosinus);
#ifdef WIN32
		if (!_finite(angle) || angle > M_PI)
			angle = 0.0;
#elif __APPLE__
		if (!isfinite(angle) || angle > M_PI)
			angle = 0.0;
#elif defined(__ANDROID__)
		if (!isfinite(angle) || angle > M_PI)
			angle = 0.0;
#else
		if (!finite(angle) || angle > M_PI)
			angle = 0.0;
#endif
		return (float)angle;
	}
}