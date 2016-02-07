//*********************************************************************
// クォータニオンクラス
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#include "DXUT.h"
#include "Math.h"
#include "Vector.h"
#include "Quaternion.h"

namespace KMT
{
	Quaternion::Quaternion(const Vector3& axis, float angle)
	{
		D3DXQuaternionRotationAxis(this, (D3DXVECTOR3*)&axis, angle);
	}

	Quaternion::Quaternion(const Vector3& axis)
	{
		D3DXQuaternionRotationAxis(this, (D3DXVECTOR3*)&axis, axis.Length() * 2);
	}

	Quaternion::Quaternion(Vector3 from, Vector3 to)
	{
		Vector3 c = from.Normalize().Cross(to.Normalize());
		float d = from.Dot(to);
		d = (d > 1 ? 1 : (d < -1 ? -1 : d));
		D3DXQuaternionRotationAxis(this, (D3DXVECTOR3*)&c, acosf(d));
	}

	Quaternion::Quaternion(Vector3 from, Vector3 to, Vector3 axis)
	{
		axis.Normalize();
		Vector3 fa = (from - from.Dot(axis) * axis).Normalize();
		Vector3 ta = (to - to.Dot(axis) * axis).Normalize();
		*this = Quaternion(from, fa) * Quaternion(fa, ta) * Quaternion(ta, to);
	}

	Quaternion::Quaternion(const Quaternion& a, const Quaternion& b, float t)
	{
		D3DXQuaternionSlerp(this, &a, &b, t);
	}

	Quaternion::Quaternion(const D3DXQUATERNION& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		w = a.w;
	}

	Quaternion::operator D3DXQUATERNION() const { return D3DXQUATERNION(x, y, z, w); }

	Quaternion& Quaternion::operator= (const Quaternion& a)
	{
		x = a.x, y = a.y, z = a.z, w = a.w;
		return *this;
	}

	Quaternion& Quaternion::Normalize()
	{
		D3DXQuaternionNormalize((D3DXQUATERNION*)this, (D3DXQUATERNION*)this);
		return *this;
	}

	Quaternion& Quaternion::Inverse()
	{
		D3DXQuaternionInverse((D3DXQUATERNION*)this, (D3DXQUATERNION*)this);
		return *this;
	}

	Quaternion& Quaternion::Delta(Quaternion a)
	{
		QuaternionDelta(this, this, &a);
		return *this;
	}
}