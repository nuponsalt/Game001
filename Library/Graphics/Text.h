//*********************************************
// テキスト表示クラス
// Last Update : 2013.12.5
// Yuta Komatsu
//*********************************************
#pragma once

#include <string>
#include <memory>
#include "../Math/Vector.h"

namespace KMT {
	// 前方宣言
	class CDebugText;
	typedef std::shared_ptr<CDebugText> CDebugTextSP;
	typedef std::weak_ptr<CDebugText> CDebugTextWP;
	
	//**デバッグ専用テキスト描画クラス**//
	class CDebugText
	{
	private:
		// フォント
		LPD3DXFONT pd3dfont;
		// 色情報(ビット)
		DWORD bitColor;

		// コンストラクタ
		CDebugText();

	public:
		// デストラクタ
		~CDebugText();

		// フォントの生成
		// nSizeX : 文字サイズ
		// wFont : 使用フォント
		static CDebugTextSP CreateText(size_t _Size, const LPCWCHAR wFont);
		// 文字列の描画
		// 引数　描画する文字列
		void drawText(const int _Left, const int _Top, const DWORD _bitColor, LPCWSTR _Text, ...);
	};

}