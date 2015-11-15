#pragma once

#include <DirectXMath.h>
using namespace DirectX;

namespace KMT {

	//class Matrix : public XMFLOAT4X4A
	//{
	//public:
	//	Matrix() { this = XMMatrixIdentity(); }
	//};

	// �s��
	class CMatrix : public D3DXMATRIX {
	public :

		// �P�ʍs��𐶐�
		CMatrix(){ D3DXMatrixIdentity( this ) ; }

		// �z��ŗv�f���w�肵�čs��𐶐�
		CMatrix( const float* f ) : D3DXMATRIX( f ) {}

		// D3DXMATRIX����̕ϊ�
		CMatrix( const D3DXMATRIX& m ) : D3DXMATRIX( m ) {}

		// �g��k�����A��]�A���s�ړ���g�ݍ��킹���s��̐���
		CMatrix(const Vector3& scale, const CQuaternion& rotation, const Vector3& position) ;

		// ���
		CMatrix& operator= ( const CMatrix& m ) ;

	} ;
}