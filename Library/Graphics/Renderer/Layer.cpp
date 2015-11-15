#include "DXUT.h"
#include "Layer.h"
#include "../GraphicsManager.h"

#include "GraphicalShape/GraphicalPlane.h"

namespace KMT {

	Layer::Layer() : _textureSurface(NULL), _depthSurface(NULL) { }

	Layer::~Layer()
	{
		// サーフェイスの開放
		SAFE_RELEASE(_textureSurface);
		// ステンシルパッファ
		SAFE_RELEASE(_depthSurface);
		// 板ポリの解放
		_board.reset();
		// カメラの解放
		_camera3D.reset();
		_camera2D.reset();

		// レンダリングリストを解放
		for(int i = 0; i < RENDER_STATE_MAX; i++)
		{
			_renderLists[i]->clear();
		}
		for(size_t i = 0; i < _renderLists.size(); i++)
		{
			SAFE_DELETE(_renderLists[i]);
		}
		_renderLists.clear();
	}

	LayerSP Layer::Create(size_t width, size_t height)
	{
		LayerSP object = LayerSP(new Layer);

		// テクスチャの生成
		object->_texture = CTexture::CreateEmpty(width, height);

		// サーフェイスの取得
		if (FAILED(object->_texture->getpd3dTexture()->GetSurfaceLevel(0, &object->_textureSurface)))
		{
			// サーフェイス取得失敗
			// エラーに対応するコード
		}

		// ステンシルバッファの作成
		if (FAILED(GraphicsManager::_device->CreateDepthStencilSurface( 
			object->_texture->getd3dImageInfo().Width,
			object->_texture->getd3dImageInfo().Height,
			D3DFMT_D16, 
			D3DMULTISAMPLE_NONE, 
			0, 
			TRUE, 
			&object->_depthSurface, 
			NULL)))
		{
			// ステンシルバッファの作成に失敗
			// エラーに対応するコードをここに書く
		}

		object->_camera3D = CCamera::Create();
		object->_camera3D->setEye(0, 0, -10.0f);

		object->_camera2D = CCamera::Create();
		float angle = object->_camera2D->getAngle();
		object->_camera2D->setEye(0, 0, -(SCREEN_HEIGHT / (2.0f * tan(angle / 2.0f))));
		// 板ポリ生成
		object->_board = GraphicalPlane::Create(width, height);
		float aspect = (float)width / height;
		object->_camera3D->setAspect(aspect);
		object->_camera2D->setAspect(aspect);
		// 板ポリにテクスチャをセット
		if(object->_board->GetTextures().size() < 1)
			object->_board->SetTexture(object->_texture);
		// レンダリングリストの作成
		for(int i = 0; i < RENDER_STATE_MAX; i++)
		{
			std::list<GraphicBehaviorWP> *list = new std::list<GraphicBehaviorWP>();
			object->_renderLists.push_back(list);
		}

		return object;
	}

	void Layer::Render()
	{
		// ステンシルバッファをセット
		GraphicsManager::_device->SetDepthStencilSurface(_depthSurface);

		// レンダリングターゲットをセット
		GraphicsManager::_device->SetRenderTarget(0, _textureSurface);

		// サーフェイスをクリア
		GraphicsManager::_device->Clear(0,
			NULL, 
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 100, 149, 237),
			1.0f,
			0);

		// カメラの更新
		_camera3D->updateView();
		_camera2D->updateView();

		//std::unordered_map<size_t, std::list<GraphicBehaviorWP>*>::iterator mapIterator;

		// 2Dソート
		_renderLists[RENDER_BACK2D]->sort(GraphicBehavior::Compare2D);
		_renderLists[RENDER_FRONT2D]->sort(GraphicBehavior::Compare2D);

		// リストアクセス用イテレータ
		std::list<GraphicBehaviorWP>::iterator listIterator;
		// 3Dソート
		for(int i = 0; i < 2; i++)
		{
			listIterator = _renderLists[RENDER_NORMAL + i]->begin();

			while(listIterator != _renderLists[RENDER_NORMAL + i]->end())
			{
				// すでにデリートされていた場合リストから除外してスキップ
				if((*listIterator).lock() == NULL)
				{
					listIterator = _renderLists[RENDER_NORMAL + i]->erase(listIterator);
					continue;
				}

				// カメラからの距離を計算
				Vector3 cameraPosition = _camera3D->getEye();
				Vector3 length = (*listIterator).lock()->Position - cameraPosition;
				(*listIterator).lock()->_cameraDistance = length.Length();

				++listIterator;
			}
		}

		// ソート
		_renderLists[RENDER_NORMAL]->sort(GraphicBehavior::Compare);
		_renderLists[RENDER_ALPHA]->sort(GraphicBehavior::CompareBack);

		// レンダリングーリストの中身をすべて描画
		for(int i = RENDER_BACK2D; i < RENDER_STATE_MAX; i++)
		{
			if(i != RENDER_ALPHA)
			{
				// Zバッファのクリア
				GraphicsManager::_device->Clear(0, 
					NULL, 
					D3DCLEAR_ZBUFFER,
					D3DCOLOR_ARGB(255, 0, 191, 255),
					1.0f,
					0);
			}

			// Zバッファ切り替え
			(i == RENDER_NORMAL || i == RENDER_ALPHA) 
				? GraphicsManager::_device->SetRenderState(D3DRS_ZENABLE, TRUE)
				: GraphicsManager::_device->SetRenderState(D3DRS_ZENABLE, FALSE);

			listIterator = _renderLists[i]->begin();
			
			while(listIterator != _renderLists[i]->end())
			{
				// すでにデリートされていた場合リストから除外してスキップ
				if((*listIterator).lock() == NULL)
				{
					listIterator = _renderLists[i]->erase(listIterator);
					continue ;
				}
				// カメラ選択
				CCameraSP camera = (i == RENDER_BACK2D || i == RENDER_FRONT2D) ? _camera2D : _camera3D;
				// 描画
				(*listIterator).lock()->Render(camera.get());
				// 次のイテレータへ
				listIterator ++;
			}
		}
	}
}