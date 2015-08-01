#include "DXUT.h"
#include "Sprite.h"
#include "../GraphicsManager.h"

namespace KMT {

	CSprite::CSprite() : vTurn(1, 1)
	{
		// スプライトの生成
		D3DXCreateSprite(CGraphicsManager::pd3dDevice, &pd3dSprite) ;
	}

	CSprite::~CSprite()
	{
		Texture.reset() ;
		SAFE_RELEASE( pd3dSprite ) ;
	}

	void CSprite::setTexture(const CTextureSP &_ptexture) 
	{
		Texture = _ptexture ;
	}

	CSpriteSP CSprite::CreateFromFile(const std::string &_path)
	{
		CSpriteSP _obj(new CSprite()) ;
		// テクスチャを設定
		_obj->LoadTextureAndAnimation(_path) ;

		return _obj ;
	}

	CSpriteSP CSprite::CreateFromFile(const std::string &_path, const int &_w_num, const int &_h_num) 
	{
		CSpriteSP _obj(new CSprite()) ;
		// テクスチャの設定
		_obj->LoadTextureAndAnimation(_path, _w_num, _h_num) ;

		return _obj ;
	}

	void CSprite::Render(const CCamera* camera)
	{
		// 描画するか
		if(!isRender)
			return;
		//---------------------------------------------------------------------------------
		//
		// ワールド行列に関する作業
		//
		D3DXMATRIX WldMtx, PosMtx, SclMtx, RotMtx; // ワールド行列、トランスレーション行列（座標）、スケール行列、回転行列
		// トランスレーション( 平行移動 ) 行列の作成
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		// スケール( 拡縮 ) 行列の作成
		D3DXMatrixScaling(&SclMtx, vTurn.x * Scale.x, vTurn.y * Scale.y, 1.0f);
		// ローテーション( 回転 ) 行列の作成 
		// ※ 今回は 2Dなので Z回転のみ
		D3DXMatrixRotationZ(&RotMtx, D3DXToRadian(vRotation.z));
		//【 !重要! 】ワールド行列の算出 
		// DirectX では ワールド行列の算出は
		// スケール * ローテーション * トランスレーション の順で行う
		WldMtx = SclMtx * RotMtx * PosMtx;
		// スプライトにワールド行列を設定
		pd3dSprite->SetTransform(&WldMtx);

		HRESULT hr;
		// Render the scene
		if(SUCCEEDED( DXUTGetD3D9Device()->BeginScene()))
		{		
			// スプライトの描画準備
			pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND); // 半透明、不透明のときは０を渡す
			//----------------------------------------------------------------------------------
			//
			// 加算ブレンドの設定
			//
			if(isAddBlend)
			{
				DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}/*else{
				CGraphicsManager::pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ) ;
			}*/

			// スプライトの描画命令
			pd3dSprite->Draw(
				// 描画に使用するテクスチャ
				Texture->getpd3dTexture(),
				// 画像の描画範囲
				&Rects[Number],	
				// サーフェイスの中心
				(D3DXVECTOR3*)&CenterPosition,
				// 描画座標(行列で制御するので0, 0, 0でOK)
				&D3DXVECTOR3(0,0,0),
				// 色
				D3DCOLOR_ARGB((int)(vColorRGBA.w * 255), (int)(vColorRGBA.x * 255), (int)(vColorRGBA.y * 255), (int)(vColorRGBA.z * 255)));

			// 加算切り替え
			pd3dSprite->End();
			// 描画終了
			V(CGraphicsManager::pd3dDevice->EndScene());
		}
	}

}