//*********************************************************************
// クォータニオンクラス
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#include "DXUT.h"
#include "Math.h"
#include "Vector.h"
#include "Quaternion.h"

namespace KMT {

	CQuaternion::CQuaternion( const CVector3& axis, float angle ) {
		D3DXQuaternionRotationAxis( this, ( D3DXVECTOR3*)&axis, angle ) ;
	}

	CQuaternion::CQuaternion( const CVector3& axis ){
		D3DXQuaternionRotationAxis( this, (D3DXVECTOR3*)&axis, axis.Length() * 2 ) ;
	}

	CQuaternion::CQuaternion( CVector3 vfrom, CVector3 vto ){
		CVector3 c = vfrom.Normalize().Cross( vto.Normalize() ) ;
		float d = vfrom.Dot( vto ) ;
		d = ( d > 1 ? 1 : ( d < - 1 ? - 1 : d ) ) ;
		D3DXQuaternionRotationAxis( this, (D3DXVECTOR3*)&c, acosf( d ) ) ;
	}

	CQuaternion::CQuaternion( CVector3 vfrom, CVector3 vto, CVector3 axis ) {
		axis.Normalize() ;
		CVector3 fa = ( vfrom - vfrom.Dot( axis ) * axis ).Normalize() ; 
		CVector3 ta = ( vto - vto.Dot( axis ) * axis ).Normalize() ;
		*this = CQuaternion( vfrom, fa ) * CQuaternion( fa, ta ) * CQuaternion ( ta, vto ) ;
	}

	CQuaternion::CQuaternion( const CQuaternion& v1, const CQuaternion& v2, float f ) {
		D3DXQuaternionSlerp( this, &v1, &v2, f ) ;
	}

	CQuaternion::CQuaternion( const D3DXQUATERNION& q ) {
		x = q.x ; 
		y = q.y ; 
		z = q.z ; 
		w = q.w ;
	}

	CQuaternion::operator D3DXQUATERNION() const { return D3DXQUATERNION( x, y, z, w ) ; }

	CQuaternion& CQuaternion::operator= ( const CQuaternion& q ) {
		x = q.x, y = q.y, z = q.z, w = q.w ;
		return *this ;
	}

	CQuaternion& CQuaternion::Normalize() {
		D3DXQuaternionNormalize( (D3DXQUATERNION*)this, (D3DXQUATERNION*)this ) ;
		return *this ;
	}

	CQuaternion& CQuaternion::Inverse() {
		D3DXQuaternionInverse( (D3DXQUATERNION*)this, (D3DXQUATERNION*)this ) ;
		return *this ;
	}

	CQuaternion& CQuaternion::Delta( CQuaternion q ) {
		QuaternionDelta( this, this, &q ) ;
		return *this ;
	}

}