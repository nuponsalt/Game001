#include "DXUT.h"
#include "BoundingVolume.h"

#include "../Extension.h"

namespace KMT {

	Plane::Plane(const CVector3& v1, const CVector3& v2, const CVector3& v3)
	{
		Normal = NormalOnPlane(v1, v2, v3); 
		Distance = Normal.Dot(v1);
	}

}