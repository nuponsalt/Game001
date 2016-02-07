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

	Quaternion RotationX( float angle ) { return Quaternion( Vector3( 1, 0, 0 ), angle ) ; }

	Quaternion RotationY( float angle ) { return Quaternion( Vector3( 0, 1, 0 ), angle ) ; }

	Quaternion RotationZ( float angle ) { return Quaternion( Vector3( 0, 0, 1 ), angle ) ; }

	Vector3 Interpolate(const Vector3 &v1, const Vector3 &v2, float f)
	{
		Vector3 _v = v1 ;
		_v.x = Interpolate(_v.x, v2.x, f) ;
		_v.y = Interpolate(_v.y, v2.y, f) ;
		_v.z = Interpolate(_v.z, v2.z, f) ;
		return _v ;
	}

	Quaternion Interpolate(const Quaternion& q1, const Quaternion& q2, float f) 
	{
		return Quaternion( q1, q2, f ) ;
	}

	Quaternion* QuaternionDelta( Quaternion *pOut, const Quaternion *pQ1, const Quaternion *pQ2 )
	{
		Quaternion q0i ;
		D3DXQuaternionInverse( &q0i, pQ1 ) ;
		Quaternion qd = q0i * (*pQ2) ;
		if ( qd.w < 0 ) {
			qd.x *= -1.0f ;
			qd.y *= -1.0f ;
			qd.z *= -1.0f ;
			qd.w *= -1.0f ;
		}
		pOut = &qd ;
		return pOut ;
	}

}