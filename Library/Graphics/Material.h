//*********************************************
// カラー管理クラス
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#include "../Math/Vector.h"

namespace KMT {

	class Material
	{
	public :		
		// カラー(ビット値)
		int _color;
		// カラー(4Dベクトル)
		Vector4 _colorRGBA;
		// コンストラクタ
		Material() : _colorRGBA(1, 1, 1, 1), _addsBlend(false) { }
		// デストラクタ
		virtual ~Material() { }
		// 取得 : 加算フラグ
		inline bool GetAddsBlend() { return _addsBlend; }
		// 設定 : 加算フラグ
		inline void SetAddsBlend(bool adds) { _addsBlend = adds; }
		// 取得 : カラー
		inline Vector4 GetColorRGBA() const { return _colorRGBA; }
		// 設定 : カラー
		inline void SetColorRGBA(const float &r, const float &g, const float &b, const float &a) { _colorRGBA = Vector4(r, g, b, a); }
		inline void SetColorRGBA(const Vector4 &rgba) { _colorRGBA = rgba; }

	protected :
		// 加算合するか、否か
		bool _addsBlend;

	};

}