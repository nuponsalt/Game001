#pragma once

#include "../Math/Vector.h"

namespace KMT {

	// 頂点情報
	class CVertex
	{
	public :
		// 頂点座標
		CVector3 Position;
		// 法線
		CVector3 Normal;
		// 頂点カラー
		DWORD	 Color;
		// テクセル座標
		CVector2 UV;

		// 定数FVF
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		// コンストラクタ
		CVertex() : Position(0, 0, 0), Normal(0, 1, 0), UV(0, 0), Color(0xFFFFFFFF) { }

	};

	struct SCREEN_VERTEX
	{
		// XYZRHW
		float x, y, z, rhw ;

		//DIFFUSE
		DWORD color ;

		// TEX1
		float tu0, tv0 ;

		const static DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 ;
	} ;

}