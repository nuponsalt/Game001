////*********************************************************************
//// デバッグ出力オブジェクトの役割を統括する
//// Last Update : 2013.12.20
//// Yuta Komatsu
////*********************************************************************
//#pragma once
//
//#include <vector>
//#include "../Graphics/Text.h"
//
//namespace KMT {
//
//	// モノステートクラス
//	class CDebug
//	{
//	public :
//		// 初期化
//		static void Initialize();
//		// 解放
//		static void Destroy();
//		// 描画文字列の追加
//		// _string : 追加する文字列
//		static void AddString(const std::string& _string);
//		// 文字列描画関数
//		static void RenderText();
//		// フォントの色をセットする
//		static void setTextColor(int);
//
//	private :
//		// 文字列描画用オブジェクト
//		static CDebugTextSP Text_;
//		// 描画文字列
//		static std::vector<std::string> Texts_;
//
//	};
//}