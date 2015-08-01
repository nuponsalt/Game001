//*********************************************
// ‰‰ŽZ‚ÌŠg’£ŠÖ”
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#include "DXUT.h"
#include "Math.h"
#include "Vector.h"
#include "Quaternion.h"

namespace KMT {

	//-----------------------------------------------------
	// ’è‹` : 3DVector Extension

	// ’·‚³
	float Vector3Length( const CVector3* pV ){
		return sqrtf( pV->x * pV->x + pV->y * pV->y + pV->z * pV->z ) ;
	}

	// ’·‚³‚Ì2æ
	float Vector3LengthSq( const CVector3 *pV ){
		return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z ;
	}

	// “àÏ
	float Vector3Dot(const CVector3 *pV1, const CVector3 *pV2){
		return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
	}

	// ŠOÏ
	CVector3* Vector3Cross( CVector3 *pOut, const CVector3 *pV1, const CVector3 *pV2 ){
		CVector3 v ;

		v.x = pV1->y * pV2->z - pV1->z * pV2->y ;
		v.y = pV1->z * pV2->x - pV1->x * pV2->z ;
		v.z = pV1->x * pV2->y - pV1->y * pV2->x ;

		*pOut = v ;
		return pOut ;
	}

	// üŒ`•âŠÔ
	CVector3* Vector3Lerp( CVector3 *pOut, const CVector3 *pV1, const CVector3 *pV2, float s ){
		pOut->x = pV1->x + s * ( pV2->x - pV1->x ) ;
		pOut->y = pV1->y + s * ( pV2->y - pV1->y ) ;
		pOut->z = pV1->z + s * ( pV2->z - pV1->z ) ;
		return pOut ;
	}

	// ³‹K‰»
	CVector3* Vector3Normalize( CVector3 *pOut, const CVector3 *pV ){
		float f = 1 / sqrtf( pV->x * pV->x + pV->y * pV->y + pV->z * pV->z ) ;
		pOut->x = pV->x * f ;
		pOut->y = pV->y * f ;
		pOut->z = pV->z * f ;
		return pOut ;
	}

	const CVector3 AXIS_X(1, 0, 0) ;
	const CVector3 AXIS_Y(0, 1, 0) ;
	const CVector3 AXIS_Z(0, 0, 1) ;

	//-----------------------------------------------------
	// Quaternion Extension

	CQuaternion RotationX( float angle ) { return CQuaternion( CVector3( 1, 0, 0 ), angle ) ; }

	CQuaternion RotationY( float angle ) { return CQuaternion( CVector3( 0, 1, 0 ), angle ) ; }

	CQuaternion RotationZ( float angle ) { return CQuaternion( CVector3( 0, 0, 1 ), angle ) ; }

	CVector3 Interpolate(const CVector3 &v1, const CVector3 &v2, float f)
	{
		CVector3 _v = v1 ;
		_v.x = Interpolate(_v.x, v2.x, f) ;
		_v.y = Interpolate(_v.y, v2.y, f) ;
		_v.z = Interpolate(_v.z, v2.z, f) ;
		return _v ;
	}

	CQuaternion Interpolate(const CQuaternion& q1, const CQuaternion& q2, float f) 
	{
		return CQuaternion( q1, q2, f ) ;
	}

	CQuaternion* QuaternionDelta( CQuaternion *pOut, const CQuaternion *pQ1, const CQuaternion *pQ2 )
	{
		CQuaternion q0i ;
		D3DXQuaternionInverse( &q0i, pQ1 ) ;
		CQuaternion qd = q0i * (*pQ2) ;
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