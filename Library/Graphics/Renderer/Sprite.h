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

	class Sprite ;
	typedef std::shared_ptr<Sprite> SpriteSP ;
	typedef std::weak_ptr<Sprite> SpriteWP ;

	class Sprite : public GraphicBehavior, public CAnimation
	{
	public :
		// 画像のどの範囲を描画するか
		RECT _rect;
		
		// デストラクタ
		~Sprite();

		// テクスチャの設定
		void SetTexture(const CTextureSP &texture);
		//* 生成 *//
		static SpriteSP Create();
		// 一枚テクスチャ
		// path : テクスチャーのファイルパス
		static SpriteSP CreateFromFile(const std::string &path);
		// アニメーションテクスチャ
		// path : テクスチャーのファイルパス
		// width : 横分割
		// height : 縦分割
		static SpriteSP CreateFromFile(const std::string& path, const int &width, const int &height);
		//* 描画 *//
		void Render(const CCamera* camera = NULL);
		
	private :
		// DirectX3Dスプライト
		LPD3DXSPRITE _sprite;
		// 反転用二次元ベクトル
		Vector2 _turnVector;
		
		// コンストラクタ
		Sprite();

	};

}