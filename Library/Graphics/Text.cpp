//*********************************************************************
// テキスト表示クラス
// Last Update : 2013.12.5
// Yuta Komatsu
//*********************************************************************
#include "DXUT.h"
#include "Text.h"

namespace KMT {

	CDebugText::CDebugText() : bitColor(0xFFFFFFFF) { }
	
	CDebugText::~CDebugText() 
	{
		SAFE_RELEASE(pd3dfont);
	}

	CDebugTextSP CDebugText::CreateText(size_t nSize, const LPCWCHAR wFont) 
	{
		CDebugTextSP ptext  = CDebugTextSP(new CDebugText);
		// フォントの情報を設定
		D3DXFONT_DESC _d3dfontDesc;
		// フォントの高さ
		_d3dfontDesc.Height = nSize;
		// フォントの幅
		_d3dfontDesc.Width = nSize / 2;
		_d3dfontDesc.Weight = 0;
		_d3dfontDesc.MipLevels = 0;
		_d3dfontDesc.Italic = FALSE;
		_d3dfontDesc.CharSet = SHIFTJIS_CHARSET;
		_d3dfontDesc.OutputPrecision = OUT_TT_ONLY_PRECIS;
		_d3dfontDesc.Quality = DEFAULT_QUALITY;
		_d3dfontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
		lstrcpy(_d3dfontDesc.FaceName, wFont);
		// フォント生成
		D3DXCreateFontIndirect(DXUTGetD3D9Device(), &_d3dfontDesc, &ptext->pd3dfont);
		// 結果のオブジェクトが戻る
		return ptext;
	}

	void CDebugText::drawText(const int _Left, const int _Top, const DWORD _bitColor, LPCWSTR _Text, ...)
	{
		// 描画位置の設定
		RECT _rect = { _Left, _Top, 0, 0 };
		// カラー保存
		bitColor = _bitColor;
		// 可変長引数から文字列に変換
		va_list va;
		va_start(va, _Text);
		WCHAR _Buffer[255] = { 0 };
		vswprintf_s(_Buffer, _Text, va);
		va_end(va);

		HRESULT hr;
		if(SUCCEEDED(DXUTGetD3D9Device()->BeginScene()))
		{
			// テキスト描画
			// _Text : 描画する文字列 (WCHAR 指定)
			// _rect : 描画範囲指定
			// nColor : 色指定
			pd3dfont->DrawText(NULL, _Buffer,	-1, &_rect,	DT_LEFT | DT_NOCLIP, _bitColor);
			// 描画終了
			V(DXUTGetD3D9Device()->EndScene());
		}
	}

}