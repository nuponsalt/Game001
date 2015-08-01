#include "DXUT.h"
#include "Line.h"

#include "../GraphicsManager.h"
#include "../View/Camera.h"

namespace KMT {

	CLineSP CLine::Create(const CVector3& _posA, const CVector3& _posB, DWORD _color)
	{
		// 生成する
		CLine* pobj = new CLine();

		// 頂点データの作成
		pobj->Vertices[0].x = _posA.x;
		pobj->Vertices[0].y = _posA.y;
		pobj->Vertices[0].z = _posA.z;
		pobj->Vertices[0].color = _color;

		pobj->Vertices[1].x = _posB.x;
		pobj->Vertices[1].y = _posB.y;
		pobj->Vertices[1].z = _posB.z;
		pobj->Vertices[1].color = _color;

		return CLineSP(pobj);
	}

	void CLine::Render(const CCamera* _camera)
	{
		HRESULT hr;

		// カメラ設定
		CGraphicsManager::pd3dDevice->SetTransform(D3DTS_VIEW, &_camera->getMatrix(CViewBehavior::VIEW));
		CGraphicsManager::pd3dDevice->SetTransform(D3DTS_PROJECTION, &_camera->getMatrix(CViewBehavior::PROJECTION));

		// ラインの描画
		{
			CGraphicsManager::pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

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
			CGraphicsManager::pd3dDevice->SetTexture(0, NULL);

			//ライティングOFF
			CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			CGraphicsManager::pd3dDevice->LightEnable(0, FALSE);
			CGraphicsManager::pd3dDevice->SetTransform(D3DTS_WORLD, &WldMtx);

			// Render the scene
			if(SUCCEEDED(CGraphicsManager::pd3dDevice->BeginScene()))
			{
				CGraphicsManager::pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, Vertices, sizeof(LineVertex));   
				V(CGraphicsManager::pd3dDevice->EndScene());
			}

			CGraphicsManager::pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
			CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			CGraphicsManager::pd3dDevice->LightEnable(0, TRUE);

		}

	}

}