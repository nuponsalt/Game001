//*********************************************
// カラー管理クラス
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#include "../Math/Vector.h"

namespace KMT {

	class CMaterial
	{
	public :
		
		// カラー(ビット値)
		int nColor;
		// カラー(4Dベクトル)
		CVector4 vColorRGBA;
		// コンストラクタ
		CMaterial() : vColorRGBA(1, 1, 1, 1), isAddBlend(false) { }
		// デストラクタ
		virtual ~CMaterial() { }
		// 取得 : 加算フラグ
		inline bool getisAddBlend() { return isAddBlend; }
		// 設定 : 加算フラグ
		inline void setisAddBlend(bool _add) { isAddBlend = _add; }
		// 取得 : カラー
		inline CVector4 getColorRGBA() const { return vColorRGBA; }
		// 設定 : カラー
		inline void setColorRGBA(const float &r, const float &g, const float &b, const float &a) { vColorRGBA = CVector4(r, g, b, a); }
		inline void setColorRGBA(const CVector4 &_v4color) { vColorRGBA = _v4color; }

	protected :
		// 加算合するか、否か
		bool isAddBlend;

	};

}