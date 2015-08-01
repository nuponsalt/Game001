//*********************************************
// スプライト描画クラス
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#pragma warning ( disable:4238 )

#include "GraphicBehavior.h"
#include "../Animation.h"
#include <memory>

namespace KMT {

	class CSprite ;
	typedef std::shared_ptr<CSprite> CSpriteSP ;
	typedef std::weak_ptr<CSprite> CSpriteWP ;

	class CSprite : public CGraphicBehavior, public CAnimation
	{
	public :
		// 画像のどの範囲を描画するか
		RECT rect;
		
		// デストラクタ
		~CSprite();

		// テクスチャの設定
		void setTexture(const CTextureSP &_texture);
		//* 生成 *//
		static CSpriteSP CreateSprite();
		// 一枚テクスチャ
		// _path : テクスチャーのファイルパス
		static CSpriteSP CreateFromFile(const std::string &_path);
		// アニメーションテクスチャ
		// _path : テクスチャーのファイルパス
		// _w_num : 横分割
		// _h_num : 縦分割
		static CSpriteSP CreateFromFile(const std::string &_path, const int &_w_num, const int &_h_num);
		//* 描画 *//
		void Render(const CCamera* camera = NULL);
		
	private :
		// DirectX3Dスプライト
		LPD3DXSPRITE pd3dSprite;
		// 反転用二次元ベクトル
		CVector2 vTurn;
		
		// コンストラクタ
		CSprite();

	};

}