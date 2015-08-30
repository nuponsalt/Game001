#pragma once

#include <DirectXMath.h>
using namespace DirectX;

namespace KMT {

	//class Matrix : public XMFLOAT4X4A
	//{
	//public:
	//	Matrix() { this = XMMatrixIdentity(); }
	//};

	// 行列
	class CMatrix : public D3DXMATRIX {
	public :

		// 単位行列を生成
		CMatrix(){ D3DXMatrixIdentity( this ) ; }

		// 配列で要素を指定して行列を生成
		CMatrix( const float* f ) : D3DXMATRIX( f ) {}

		// D3DXMATRIXからの変換
		CMatrix( const D3DXMATRIX& m ) : D3DXMATRIX( m ) {}

		// 拡大縮小率、回転、平行移動を組み合わせた行列の生成
		CMatrix(const Vector3& scale, const CQuaternion& rotation, const Vector3& position) ;

		// 代入
		CMatrix& operator= ( const CMatrix& m ) ;

	} ;
}