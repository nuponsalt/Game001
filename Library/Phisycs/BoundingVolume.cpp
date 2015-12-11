#include "DXUT.h"
#include "BoundingVolume.h"

#include "../Extension.h"

namespace KMT {

	Plane::Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Normal = NormalOnPlane(v1, v2, v3); 
		Distance = Normal.Dot(v1);
	}

}