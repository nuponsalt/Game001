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
	class DebugText;
	typedef std::shared_ptr<DebugText> DebugTextSP;
	typedef std::weak_ptr<DebugText> DebugTextWP;
	
	//**デバッグ専用テキスト描画クラス**//
	class DebugText
	{
	public:
		// デストラクタ
		~DebugText();

		// フォントの生成
		// nSizeX : 文字サイズ
		// wFont : 使用フォント
		static DebugTextSP CreateText(size_t size, const LPCWCHAR fontName);
		// 文字列の描画
		// 引数　描画する文字列
		void drawText(const int _Left, const int _Top, const DWORD _bitColor, LPCWSTR _Text, ...);

	private:
		// フォント
		LPD3DXFONT _font;
		// 色情報(ビット)
		DWORD _color;

		// コンストラクタ
		DebugText();	
	};

}