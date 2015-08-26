#include "DXUT.h"
#include "Line.h"

#include "../GraphicsManager.h"
#include "../View/Camera.h"

namespace KMT {

	LineSP Line::Create(const Vector3& positionA, const Vector3& positionB, DWORD color)
	{
		// 生成する
		Line* object = new Line();

		// 頂点データの作成
		object->_vertices[0]._x = positionA.x;
		object->_vertices[0]._y = positionA.y;
		object->_vertices[0]._z = positionA.z;
		object->_vertices[0]._color = color;

		object->_vertices[1]._x = positionB.x;
		object->_vertices[1]._y = positionB.y;
		object->_vertices[1]._z = positionB.z;
		object->_vertices[1]._color = color;

		return LineSP(object);
	}

	void Line::Render(const CCamera* _camera)
	{
		HRESULT hr;

		// カメラ設定
		GraphicsManager::_device->SetTransform(D3DTS_VIEW, &_camera->getMatrix(CViewBehavior::VIEW));
		GraphicsManager::_device->SetTransform(D3DTS_PROJECTION, &_camera->getMatrix(CViewBehavior::PROJECTION));

		// ラインの描画
		{
			GraphicsManager::_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

			// ワールドマトリクス設定
			D3DXMATRIX SclMtx, RotMtx, PosMtx, WldMtx ;
			D3DXMatrixScaling(&SclMtx, 1, 1, 1);
			D3DXMatrixIdentity(&RotMtx);
			D3DXMatrixTranslation(&PosMtx, 0, 0, 0);
			WldMtx = SclMtx * RotMtx * PosMtx;

			// 通常合成
			//GraphicsManager::m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ) ;
			//GraphicsManager::m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 ) ;
			//GraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ) ;
			//GraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ) ;	
			//GraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ) ;

			// テクスチャは使わない
			GraphicsManager::_device->SetTexture(0, NULL);

			//ライティングOFF
			GraphicsManager::_device->SetRenderState(D3DRS_LIGHTING, FALSE);
			GraphicsManager::_device->LightEnable(0, FALSE);
			GraphicsManager::_device->SetTransform(D3DTS_WORLD, &WldMtx);

			// Render the scene
			if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
			{
				GraphicsManager::_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, _vertices, sizeof(LineVertex));   
				V(GraphicsManager::_device->EndScene());
			}

			GraphicsManager::_device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
			GraphicsManager::_device->SetRenderState(D3DRS_LIGHTING, TRUE);
			GraphicsManager::_device->LightEnable(0, TRUE);

		}

	}

}