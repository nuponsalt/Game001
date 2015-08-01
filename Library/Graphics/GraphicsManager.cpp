#include "DXUT.h"
#include "GraphicsManager.h"

#include "Shader/Shader.h"
#include "Shader/Lambert/ShaderLambert.h"
#include "Shader/HalfLambert/ShaderHalfLambert.h"
#include "Shader/Phong/ShaderPhong.h"
#include "Shader/Toon/ShaderToon.h"
#include "Texture.h"

namespace KMT {

	IDirect3DDevice9* CGraphicsManager::pd3dDevice;

	LPDIRECT3DSURFACE9 CGraphicsManager::pd3dBackBufferSurface;

	LPDIRECT3DSURFACE9 CGraphicsManager::pd3dBackDepthSurface;

	void CGraphicsManager::Initialize(IDirect3DDevice9 *_pd3dDevice)
	{
		// デバイスポインタの取得
		pd3dDevice = _pd3dDevice;

		// バックバッファのサーフェイスを取得
		pd3dDevice->GetRenderTarget(0, &pd3dBackBufferSurface);
		
		// 深度バッファのサーフェイスを取得
		pd3dDevice->GetDepthStencilSurface(&pd3dBackDepthSurface);

		//-------------------------------------------------------------
		// シェーダーの初期読み込み

		//* シェーディング無設定 *//
		CShaderNormal::CreateShader();
		//* ランバート拡散反射シェーディング *//
		CShaderLambert::CreateShader();
		//* ハーフランバートシェーダー *//
		CShaderHalfLambert::CreateShader();
		//* フォンシェーダー *//
		CShaderPhong::CreateShader();
		//* トゥーンシェーダー *//
		CShaderToon::CreateShader();
	}
	
	void CGraphicsManager::Destroy()
	{
		// バックバッファのサーフェイスを解放
		SAFE_RELEASE(pd3dBackBufferSurface);
		// 深度バッファのサーフェイスを解放
		SAFE_RELEASE(pd3dBackDepthSurface);
		// シェーダーの開放
		CShader::Destroy();
		// テクスチャの開放
		CTexture::Destroy();
	}

	void CGraphicsManager::setRenderTargetToBackBuffer()
	{
		// ステンシルバッファをセット
		pd3dDevice->SetDepthStencilSurface(pd3dBackDepthSurface);
		// レンダリングターゲットをセット
		pd3dDevice->SetRenderTarget(0, pd3dBackBufferSurface);
		// サーフェイスをクリア
		pd3dDevice->Clear(0, 
			NULL, 
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0, 20, 40, 50),
			1.0f,
			0);
	}

}