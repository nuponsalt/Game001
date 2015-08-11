#include "DXUT.h"
#include "Layer.h"
#include "../GraphicsManager.h"

#include "GraphicalShape/GraphicalPlane.h"

namespace KMT {

	CLayer::CLayer() : pd3dTextureSurface(NULL), pd3dDepthSurface(NULL) { }

	CLayer::~CLayer()
	{
		// サーフェイスの開放
		SAFE_RELEASE(pd3dTextureSurface);
		// ステンシルパッファ
		SAFE_RELEASE(pd3dDepthSurface);
		// 板ポリの解放
		board.reset();
		// カメラの解放
		Camera3D.reset();
		Camera2D.reset();

		// レンダリングリストを解放
		//-----------------------------------------
		for(int i = 0; i < RENDER_STATE_MAX; i++)
		{
			render_lists[i]->clear();
		}
		for(size_t i = 0; i < render_lists.size(); i++)
		{
			SAFE_DELETE(render_lists[i]);
		}
		render_lists.clear();
	}

	CLayerSP CLayer::CreateLayer(size_t width, size_t height)
	{
		CLayerSP pRenderTag = CLayerSP(new CLayer);

		// テクスチャの生成
		pRenderTag->Texture = CTexture::CreateEmpty(width, height);

		// サーフェイスの取得
		if (FAILED(pRenderTag->Texture->getpd3dTexture()->GetSurfaceLevel(0, &pRenderTag->pd3dTextureSurface)))
		{
			// サーフェイス取得失敗
			// エラーに対応するコード
		}

		// ステンシルバッファの作成
		if (FAILED(CGraphicsManager::pd3dDevice->CreateDepthStencilSurface( 
			pRenderTag->Texture->getd3dImageInfo().Width,
			pRenderTag->Texture->getd3dImageInfo().Height,
			D3DFMT_D16, 
			D3DMULTISAMPLE_NONE, 
			0, 
			TRUE, 
			&pRenderTag->pd3dDepthSurface, 
			NULL)))
		{
			// ステンシルバッファの作成に失敗
			// エラーに対応するコードをここに書く
		}

		pRenderTag->Camera3D = CCamera::Create();
		pRenderTag->Camera3D->setEye(0, 0, -10.0f);

		pRenderTag->Camera2D = CCamera::Create();
		float angle = pRenderTag->Camera2D->getAngle();
		pRenderTag->Camera2D->setEye(0, 0, -(SCREEN_HEIGHT / (2.0f * tan(angle / 2.0f))));
		// 板ポリ生成
		pRenderTag->board = CGraphicalPlane::Create(width, height);
		float aspect = (float)width / height;
		pRenderTag->Camera3D->setAspect(aspect);
		pRenderTag->Camera2D->setAspect(aspect);
		// 板ポリにテクスチャをセット
		if(pRenderTag->board->getptextures().size() < 1)
			pRenderTag->board->SetTexture(pRenderTag->Texture);
		// レンダリングリストの作成
		for(int i = 0; i < RENDER_STATE_MAX; i++)
		{
			std::list<CGraphicBehaviorWP> *list = new std::list<CGraphicBehaviorWP>();
			pRenderTag->render_lists.push_back(list);
		}

		return pRenderTag;
	}

	void CLayer::Render()
	{
		// ステンシルバッファをセット
		CGraphicsManager::pd3dDevice->SetDepthStencilSurface(pd3dDepthSurface);

		// レンダリングターゲットをセット
		CGraphicsManager::pd3dDevice->SetRenderTarget(0, pd3dTextureSurface);

		// サーフェイスをクリア
		CGraphicsManager::pd3dDevice->Clear(0,
			NULL, 
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 100, 149, 237),
			1.0f,
			0);


		// カメラの更新
		Camera3D->updateView();
		Camera2D->updateView();

		std::unordered_map<size_t, std::list<CGraphicBehaviorWP>*>::iterator hmit;

		// 2Dソート
		render_lists[RENDER_BACK2D]->sort(CGraphicBehavior::comp2D);
		render_lists[RENDER_FRONT2D]->sort(CGraphicBehavior::comp2D);

		// リストアクセス用イテレータ
		std::list<CGraphicBehaviorWP>::iterator it;

		// 3Dソート
		//-------------------------------------------------
		for(int i = 0; i < 2; i++)
		{
			it = render_lists[RENDER_NORMAL + i]->begin();

			while(it != render_lists[RENDER_NORMAL + i]->end())
			{
				// すでにデリートされていた場合リストから除外してスキップ
				if((*it).lock() == NULL)
				{
					it = render_lists[RENDER_NORMAL + i]->erase(it);
					continue;
				}

				// カメラからの距離を計算
				CVector3 campos = Camera3D->getEye();
				CVector3 vec = (*it).lock()->Position - campos;
				(*it).lock()->CameraDistance = vec.Length();

				++it;
			}
		}

		// ソート
		render_lists[RENDER_NORMAL]->sort(CGraphicBehavior::comp);
		render_lists[RENDER_ALPHA]->sort(CGraphicBehavior::compBack);
		//-------------------------------------------------------

		// レンダリングーリストの中身をすべて描画
		//-------------------------------------------------------
		for(int i = RENDER_BACK2D; i < RENDER_STATE_MAX; i++)
		{
			if(i != RENDER_ALPHA)
			{
				// Zバッファのクリア
				CGraphicsManager::pd3dDevice->Clear(0, 
					NULL, 
					D3DCLEAR_ZBUFFER,
					D3DCOLOR_ARGB(255, 0, 191, 255),
					1.0f,
					0);
			}

			// Zバッファ切り替え
			(i == RENDER_NORMAL || i == RENDER_ALPHA) 
				? CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE)
				: CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

			it = render_lists[i]->begin();
			
			while(it != render_lists[i]->end())
			{
				// すでにデリートされていた場合リストから除外してスキップ
				if((*it).lock() == NULL)
				{
					it = render_lists[i]->erase(it);
					continue ;
				}
				// カメラ選択
				CCameraSP camera = (i == RENDER_BACK2D || i == RENDER_FRONT2D) ? Camera2D : Camera3D;
				// 描画
				(*it).lock()->Render(camera.get());
				// 次のイテレータへ
				it ++;
			}
		}
	}
}