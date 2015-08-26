#pragma once 

#include "Texture.h"
#include "../Math/Vector.h"

namespace KMT 
{

	// アニメーション再生モード
	enum PlayMode{
		// フレーム指定
		SELECT_FRAME,			
		// 一度だけ再生
		PLAY_ONCE,
		// ループ再生
		PLAY_LOOP	
	};

	class Animation 
	{
	public :
		// コンストラクタ
		Animation();
		// デストラクタ
		virtual ~Animation();

		// テクスチャを画像ファイルから読み込む
		// path : 画像のファイルパス
		// divisionX : 横分割
		// divisionY : 縦分割
		void LoadTextureAndAnimation(const std::string &path, const int &divisionX = 1, const int &divisionY = 1, const DWORD &filter = D3DX_FILTER_NONE);
		
		//* 取得 *//
		// 現在のアニメーション番号を取得する
		inline int GetAnimationNumber() const { return _number; }

		//* 設定 *//
		// 分割数
		inline void SetDivisionWidth(const int &num) { _divisionWidth = num; }
		inline void SetDivisionHeight(const int &num) { _divisionHeight = num; }
		// 再生速度
		inline void SetAnimationSpeed(int speed) { _speed = speed; }
		// 再生モード
		inline void SetPlayMode(PlayMode mode) { _currentPlayMode = mode; }

		// アニメーション制御
		// mode : 再生モード
		// frame : フレーム数
		// 戻り値 : 再生中か否か(falseは再生中、trueは再生終了)
		// 備考 : フレーム指定モードの場合は第二引数にフレーム番号を、それ以外のモードでは何フレーム毎にアニメーションさせるかを渡す
		const bool UpdateAnimation(const int &frame = 0);

	protected :
		// テクスチャー
		TextureSP _texture;
		// 分割数(width:横, height:縦)
		int _divisionWidth, _divisionHeight;
		// 描画範囲
		std::vector<RECT> _rects;
		// 画像分割後のサイズ
		Vector3 _imageSize;
		// 描画中心座標
		Vector3 _centerPosition;
		// アニメーションの速さ
		int _speed;
		// 表示するアニメーション番号
		int _number;
		// アニメーション用のカウンタ
		int _count;
		// 再生モード
		PlayMode _currentPlayMode;
		
	};
}