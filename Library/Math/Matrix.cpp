#include "DXUT.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace KMT {

	CMatrix::CMatrix(const CVector3& scale, const CQuaternion& rotation, const CVector3& position) {
		D3DXMatrixTransformation(this, 0, 0, (D3DXVECTOR3*)&scale, 0, (D3DXQUATERNION*)&rotation, (D3DXVECTOR3*)&position) ;
	}

	// ‘ã“ü
	CMatrix& CMatrix::operator= ( const CMatrix& m ) {
		memcpy( this, &m, sizeof(CMatrix) ) ;
		return *this;
	}

}