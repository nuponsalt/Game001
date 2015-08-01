#include "DXUT.h"
//#include "Debug.h"
//
//#include "../Extension.h"
//
//namespace KMT {
//
//	//-----------------------------------
//	// 静的変数宣言
//	CTextDebugSP CDebug::text ;
//	std::vector< std::string > CDebug::strings ;
//
//	void CDebug::Initialize()
//	{
//		// フォントの生成
//		text = CTextDebug::Create( 14, 20, L"ＭＳ ゴシック" ) ;
//	}
//	
//	void CDebug::Destroy()
//	{
//		text.reset() ;
//	}
//
//	void CDebug::AddString( const std::string _string ) 
//	{
//		// 文字列を追加
//		strings.push_back( _string ) ;
//	}
//
//	void CDebug::RenderText()
//	{
//		text->setRect( 0, 0 ) ;
//		// 登録されている文字列を全て描画
//		for( size_t i = 0 ; i < strings.size() ; ++i )
//		{
//			// WCHARに変換してから描画
//			WCHAR buff_str[256] = {0} ;
//			DXconvAnsiToWide( buff_str, strings[i].c_str(), ( strings[i].length()+1) ) ;
//			text->drawText( buff_str ) ;
//
//			text->setRect( 0, ( i + 1 ) * 20 ) ;
//
//		}
//	}
//
//	void CDebug::setTextColor( int _col )
//	{
//		// カラーをセット
//		text->setColor( _col ) ;
//	}
//
//}