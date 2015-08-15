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
		RECT _rect;
		
		// デストラクタ
		~CSprite();

		// テクスチャの設定
		void SetTexture(const CTextureSP &texture);
		//* 生成 *//
		static CSpriteSP Create();
		// 一枚テクスチャ
		// _path : テクスチャーのファイルパス
		static CSpriteSP CreateFromFile(const std::string &path);
		// アニメーションテクスチャ
		// path : テクスチャーのファイルパス
		// width : 横分割
		// height : 縦分割
		static CSpriteSP CreateFromFile(const std::string& path, const int &width, const int &height);
		//* 描画 *//
		void Render(const CCamera* camera = NULL);
		
	private :
		// DirectX3Dスプライト
		LPD3DXSPRITE _sprite;
		// 反転用二次元ベクトル
		CVector2 _turnVector;
		
		// コンストラクタ
		CSprite();

	};

}