//*********************************************
// 演算の拡張関数
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#include "DXUT.h"
#include "Math.h"
#include "Vector.h"
#include "Quaternion.h"

namespace KMT
{
	//-----------------------------------------------------
	// Quaternion Extension

	Quaternion RotationX(float angle) { return Quaternion(Vector3(1, 0, 0), angle); }

	Quaternion RotationY(float angle) { return Quaternion(Vector3(0, 1, 0), angle); }

	Quaternion RotationZ(float angle) { return Quaternion(Vector3(0, 0, 1), angle); }

	Quaternion SphericalLerp(const Quaternion& q1, const Quaternion& q2, float f)
	{
		return Quaternion(q1, q2, f);
	}

	Quaternion* QuaternionDelta(Quaternion *out, const Quaternion *a, const Quaternion *b)
	{
		Quaternion inverse;
		D3DXQuaternionInverse(&inverse, a);
		Quaternion delta = inverse * (*b);
		if (delta.w < 0) {
			delta.x *= -1.0f;
			delta.y *= -1.0f;
			delta.z *= -1.0f;
			delta.w *= -1.0f;
		}
		out = &delta;
		return out;
	}

}