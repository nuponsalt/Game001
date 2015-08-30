#include "DXUT.h"
#include "GraphicsManager.h"

#include "Shader/Shader.h"
#include "Shader/Lambert/ShaderLambert.h"
#include "Shader/HalfLambert/ShaderHalfLambert.h"
#include "Shader/Phong/ShaderPhong.h"
#include "Shader/Toon/ShaderToon.h"
#include "Texture.h"

namespace KMT {

	IDirect3DDevice9* GraphicsManager::_device;

	LPDIRECT3DSURFACE9 GraphicsManager::_backBufferSurface;

	LPDIRECT3DSURFACE9 GraphicsManager::_backDepthSurface;

	void GraphicsManager::Initialize(IDirect3DDevice9* device)
	{
		// デバイスポインタの取得
		_device = device;

		// バックバッファのサーフェイスを取得
		_device->GetRenderTarget(0, &_backBufferSurface);
		
		// 深度バッファのサーフェイスを取得
		_device->GetDepthStencilSurface(&_backDepthSurface);

		//-------------------------------------------------------------
		// シェーダーの初期読み込み

		//* シェーディング無設定 *//
		ShaderNormal::Create();
		//* ランバート拡散反射シェーディング *//
		ShaderLambert::Create();
		//* ハーフランバートシェーダー *//
		ShaderHalfLambert::Create();
		//* フォンシェーダー *//
		ShaderPhong::Create();
		//* トゥーンシェーダー *//
		ShaderToon::Create();
	}
	
	void GraphicsManager::Destroy()
	{
		// バックバッファのサーフェイスを解放
		SAFE_RELEASE(_backBufferSurface);
		// 深度バッファのサーフェイスを解放
		SAFE_RELEASE(_backDepthSurface);
		// シェーダーの開放
		Shader::Destroy();
		// テクスチャの開放
		CTexture::Destroy();
	}

	void GraphicsManager::SetRenderTargetToBackBuffer()
	{
		// ステンシルバッファをセット
		_device->SetDepthStencilSurface(_backDepthSurface);
		// レンダリングターゲットをセット
		_device->SetRenderTarget(0, _backBufferSurface);
		// サーフェイスをクリア
		_device->Clear(0, 
			NULL, 
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0, 20, 40, 50),
			1.0f,
			0);
	}

}