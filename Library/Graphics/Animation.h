#pragma once 

#include "Texture.h"
#include "../Math/Vector.h"

namespace KMT 
{

	// アニメーション再生モード
	enum PLAYMODE{
		// フレーム指定
		SELECT_FRAME,			
		// 一度だけ再生
		PLAY_ONCE,
		// ループ再生
		PLAY_LOOP	
	};

	class CAnimation 
	{
	public :
		// コンストラクタ
		CAnimation();
		// デストラクタ
		virtual ~CAnimation();

		// テクスチャを画像ファイルから読み込む
		// _path : 画像のファイルパス
		// _x_num : 横分割
		// _y_num : 縦分割
		void LoadTextureAndAnimation(const std::string &_path, const int &_x_num = 1, const int &_y_num = 1, const DWORD &dwFilter = D3DX_FILTER_NONE);
		
		//* 取得 *//
		// 現在のアニメーション番号を取得する
		inline int GetAnimationNumber() const { return Number; }
		//* 設定 *//
		// 分割数
		inline void setDivideW(const int &_num) { DivideWidth = _num; }
		inline void setDivideH(const int &_num) { DivideHeight = _num; }
		// 再生速度
		inline void setAnimationSpeed(int _speed) { Speed = _speed; }
		// 再生モード
		inline void setPlayMode(PLAYMODE mode) { CurrentPlayMode = mode; }

		// アニメーション制御
		// _mode : 再生モード
		// frame : フレーム数
		// 戻り値 : 再生中か否か(falseは再生中、trueは再生終了)
		// 備考 : フレーム指定モードの場合は第二引数にフレーム番号を、それ以外のモードでは何フレーム毎にアニメーションさせるかを渡す
		const bool UpdateAnimation(const int &frame = 0);

	protected :
		// テクスチャー
		CTextureSP Texture;
		// 分割数(Width:横, Height:縦)
		int DivideWidth, DivideHeight;
		// 描画範囲
		std::vector<RECT> Rects;
		// 画像分割後のサイズ
		CVector3 ImageSize;
		// 描画中心座標
		CVector3 CenterPosition;
		// アニメーションの速さ
		int Speed;
		// 表示するアニメーション番号
		int Number;
		// アニメーション用のカウンタ
		int Count;

		// 再生モード
		PLAYMODE CurrentPlayMode;
		
	};
}