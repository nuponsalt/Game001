//*********************************************************************
// テキスト表示クラス
// Last Update : 2013.12.5
// Yuta Komatsu
//*********************************************************************
#include "DXUT.h"
#include "Text.h"
#include "GraphicsManager.h"

namespace KMT {

	DebugText::DebugText() : _color(0xFFFFFFFF) { }
	
	DebugText::~DebugText() 
	{
		SAFE_RELEASE(_font);
	}

	DebugTextSP DebugText::CreateText(size_t size, const LPCWCHAR fontName) 
	{
		DebugTextSP text  = DebugTextSP(new DebugText);
		// フォントの情報を設定
		D3DXFONT_DESC fontDesc;
		// フォントの高さ
		fontDesc.Height = size;
		// フォントの幅
		fontDesc.Width = size / 2;
		fontDesc.Weight = 0;
		fontDesc.MipLevels = 0;
		fontDesc.Italic = FALSE;
		fontDesc.CharSet = SHIFTJIS_CHARSET;
		fontDesc.OutputPrecision = OUT_TT_ONLY_PRECIS;
		fontDesc.Quality = DEFAULT_QUALITY;
		fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
		lstrcpy(fontDesc.FaceName, fontName);
		// フォント生成
		D3DXCreateFontIndirect(GraphicsManager::_device, &fontDesc, &text->_font);
		// 結果のオブジェクトが戻る
		return text;
	}

	void DebugText::drawText(const int left, const int top, const DWORD color, LPCWSTR text, ...)
	{
		// 描画位置の設定
		RECT rect = { left, top, 0, 0 };
		// カラー保存
		_color = color;
		// 可変長引数から文字列に変換
		va_list va;
		va_start(va, text);
		WCHAR buffer[255] = { 0 };
		vswprintf_s(buffer, text, va);
		va_end(va);

		HRESULT hr;
		if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
		{
			// テキスト描画
			// text : 描画する文字列 (WCHAR 指定)
			// rect : 描画範囲指定
			// nColor : 色指定
			_font->DrawText(NULL, buffer,	-1, &rect,	DT_LEFT | DT_NOCLIP, color);
			// 描画終了
			V(GraphicsManager::_device->EndScene());
		}
	}

}