#pragma once

#include <DirectXMath.h>
using namespace DirectX;

namespace KMT
{
	// 行列
	class Matrix : public D3DXMATRIX
	{
	public :
		// 単位行列を生成
		Matrix(){ D3DXMatrixIdentity(this); }
		// 配列で要素を指定して行列を生成
		Matrix(const float* f) : D3DXMATRIX(f) { }
		// D3DXMATRIXからの変換
		Matrix(const D3DXMATRIX& m) : D3DXMATRIX(m) { }
		// XMVECTORからの変換
		Matrix(const XMMATRIX& m)
		{
			_11 = XMVectorGetX(m.r[0]);
			_12 = XMVectorGetY(m.r[0]);
			_13 = XMVectorGetZ(m.r[0]);
			_14 = XMVectorGetW(m.r[0]);

			_21 = XMVectorGetX(m.r[1]);
			_22 = XMVectorGetY(m.r[1]);
			_23 = XMVectorGetZ(m.r[1]);
			_24 = XMVectorGetW(m.r[1]);

			_31 = XMVectorGetX(m.r[2]);
			_32 = XMVectorGetY(m.r[2]);
			_33 = XMVectorGetZ(m.r[2]);
			_34 = XMVectorGetW(m.r[2]);

			_41 = XMVectorGetX(m.r[3]);
			_42 = XMVectorGetY(m.r[3]);
			_43 = XMVectorGetZ(m.r[3]);
			_44 = XMVectorGetW(m.r[3]);			
		}
		// 拡大縮小率、回転、平行移動を組み合わせた行列の生成
		Matrix(const Vector3& scale, const Quaternion& rotation, const Vector3& position);
		// 代入
		Matrix& operator= (const Matrix& m);
	};
}