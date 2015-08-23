//*********************************************************************
// オフスクリーンレンダリングクラス
// Last Update : 2014.2.5
// Yuta Komatsu
//*********************************************************************
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <list>

#include "GraphicBehavior.h"
#include "GraphicalShape/GraphicalPlane.h"
#include "../View/Camera.h"

#include "../../Extension.h"


namespace KMT {

	enum RenderState
	{
		RENDER_BACK2D,
		RENDER_NORMAL,
		RENDER_ALPHA,
		RENDER_FRONT2D,
		RENDER_STATE_MAX
	};

	class Layer;
	typedef std::shared_ptr<Layer> LayerSP;
	typedef std::weak_ptr<Layer> LayerWP;

	class Layer
	{
	public :
		// カメラ
		CCameraSP _camera3D;
		CCameraSP _camera2D;
		// 板ポリモデル
		GPlaneSP _board;

		// デストラクタ
		~Layer();

		// 生成
		static LayerSP Create(size_t width, size_t height);
		// レンダリングリストの中身をすべて描画
		void Render();
		// レンダリングリストにオブジェクトを追加する
		// object 追加するオブジェクト
		// list どのレンダリングリストに追加するか
		inline void Add(GraphicBehaviorWP object, RenderState list) 
		{
			_renderLists[list]->push_back(object);
		}

	private :
		// レンダリングリスト(描画リストのハッシュマップ)
		std::vector<std::list<GraphicBehaviorWP>*> _renderLists;
		// レンダリング対象のテクスチャ
		CTextureSP _texture;
		// 上記レンダリング対象のテクスチャのサーフェイス
		LPDIRECT3DSURFACE9 _textureSurface;
		// レンダリング対象のテクスチャ用深度バッファ
		LPDIRECT3DSURFACE9 _depthSurface;

		// コンストラクタ
		Layer();

	};

} 