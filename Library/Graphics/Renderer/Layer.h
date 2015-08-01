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

	class CLayer;
	typedef std::shared_ptr<CLayer> CLayerSP;
	typedef std::weak_ptr<CLayer> CLayerWP;

	class CLayer
	{
	public :
		// カメラ
		CCameraSP Camera3D;
		CCameraSP Camera2D;
		// 板ポリモデル
		CGPlaneSP board;

		// デストラクタ
		~CLayer();

		// 生成
		static CLayerSP CreateLayer(size_t width, size_t height);
		// レンダリングリストの中身をすべて描画
		void Render();
		// レンダリングリストにオブジェクトを追加する
		// _obj 追加するオブジェクト
		// _list どのレンダリングリストに追加するか
		inline void AddObject(CGraphicBehaviorWP obj, RenderState list)
		{
			render_lists[list]->push_back(obj);
		}

	private :
		// レンダリングリスト(描画リストのハッシュマップ)
		std::vector<std::list<CGraphicBehaviorWP>*> render_lists;
		// レンダリング対象のテクスチャ
		CTextureSP Texture;
		// 上記レンダリング対象のテクスチャのサーフェイス
		LPDIRECT3DSURFACE9 pd3dTextureSurface;
		// レンダリング対象のテクスチャ用深度バッファ
		LPDIRECT3DSURFACE9 pd3dDepthSurface;

		// コンストラクタ
		CLayer();

	};

} 