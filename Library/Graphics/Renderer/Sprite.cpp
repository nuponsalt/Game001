#include "DXUT.h"
#include "Sprite.h"
#include "../GraphicsManager.h"

namespace KMT {

	Sprite::Sprite() : _turnVector(1, 1)
	{
		// スプライトの生成
		D3DXCreateSprite(GraphicsManager::_device, &_sprite) ;
	}

	Sprite::~Sprite()
	{
		_texture.reset() ;
		SAFE_RELEASE( _sprite ) ;
	}

	void Sprite::SetTexture(const TextureSP &texture) 
	{
		_texture = texture ;
	}

	SpriteSP Sprite::CreateFromFile(const std::string &path)
	{
		SpriteSP object(new Sprite()) ;
		// テクスチャを設定
		object->LoadTextureAndAnimation(path) ;

		return object ;
	}

	SpriteSP Sprite::CreateFromFile(const std::string &path, const int &width, const int &height) 
	{
		SpriteSP object(new Sprite()) ;
		// テクスチャの設定
		object->LoadTextureAndAnimation(path, width, height) ;

		return object ;
	}

	void Sprite::Render(const Camera* camera)
	{
		// 描画するか
		if(!_renders)
			return;

		// ワールド行列に関する作業
		D3DXMATRIX WldMtx, PosMtx, SclMtx, RotMtx; 
		// トランスレーション( 平行移動 ) 行列の作成
		D3DXMatrixTranslation(&PosMtx, _position.x, _position.y, _position.z);
		// スケール( 拡縮 ) 行列の作成
		D3DXMatrixScaling(&SclMtx, _turnVector.x * _scale.x, _turnVector.y * _scale.y, 1.0f);
		// ローテーション( 回転 ) 行列の作成 
		// ※ 今回は 2Dなので Z回転のみ
		D3DXMatrixRotationZ(&RotMtx, D3DXToRadian(_eulerAngles.z));
		//【 !重要! 】ワールド行列の算出 
		// DirectX では ワールド行列の算出は
		// スケール * ローテーション * トランスレーション の順で行う
		WldMtx = SclMtx * RotMtx * PosMtx;
		// スプライトにワールド行列を設定
		_sprite->SetTransform(&WldMtx);

		HRESULT hr;
		// Render the scene
		if(SUCCEEDED( DXUTGetD3D9Device()->BeginScene()))
		{		
			// スプライトの描画準備
			_sprite->Begin(D3DXSPRITE_ALPHABLEND); // 半透明、不透明のときは０を渡す
			//----------------------------------------------------------------------------------
			//
			// 加算ブレンドの設定
			//
			if(_addsBlend)
			{
				DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}/*else{
				GraphicsManager::_device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ) ;
			}*/

			// スプライトの描画命令
			_sprite->Draw(
				// 描画に使用するテクスチャ
				_texture->GetTextureData(),
				// 画像の描画範囲
				&_rects[_number],	
				// サーフェイスの中心
				(D3DXVECTOR3*)&_centerPosition,
				// 描画座標(行列で制御するので0, 0, 0でOK)
				&D3DXVECTOR3(0,0,0),
				// 色
				D3DCOLOR_ARGB((int)(_colorRGBA.w * 255), (int)(_colorRGBA.x * 255), (int)(_colorRGBA.y * 255), (int)(_colorRGBA.z * 255)));

			// 加算切り替え
			_sprite->End();
			// 描画終了
			V(GraphicsManager::_device->EndScene());
		}
	}

}