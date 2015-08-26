#pragma once

#include "../Math/Vector.h"

namespace KMT {

	// 頂点情報
	class Vertex
	{
	public :
		// 頂点座標
		Vector3 _position;
		// 法線
		Vector3 _normal;
		// 頂点カラー
		DWORD	 _color;
		// テクセル座標
		Vector2 _uv;

		// 定数FVF
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		// コンストラクタ
		Vertex() : _position(0, 0, 0), _normal(0, 1, 0), _uv(0, 0), _color(0xFFFFFFFF) { }

	};

	struct SCREEN_VERTEX
	{
		// XYZRHW
		float _x, _y, _z, _rhw ;

		//DIFFUSE
		DWORD _color ;

		// TEX1
		float _u, _v ;

		static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 ;
	} ;

}