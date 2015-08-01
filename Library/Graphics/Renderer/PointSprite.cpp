#include "DXUT.h"
#include "PointSprite.h"

#include "../Shader/Particle/Particle.h"
#include "../GraphicsManager.h"
#include "../View/Camera.h"
#include "../../Extension.h"

namespace KMT {

#define FRAND()	((float)rand() / 32768.0f)

	CPointSprite::CPointSprite(const std::string& path, const WORD numPoint, const float size) : pointNumber(numPoint), pointSize(size)
	{
		Path = path;

		d3dCull = D3DCULL_NONE;

		// 頂点バッファの作成
		if(FAILED(CGraphicsManager::pd3dDevice->CreateVertexBuffer(
			sizeof(POINTSPRITE) * pointNumber, 
			D3DUSAGE_NPATCHES | D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC, 
			POINTSPRITE::FVF,
			D3DPOOL_DEFAULT,
			&pd3dVertexBuffer,
			NULL
			)))
			MessageBox(NULL, TEXT("POINTSPRITE's VertexBuffer Create Err"), NULL, MB_OK | MB_ICONSTOP);

		// 頂点情報の初期化
		pPOINTSPRITE pPS;
		if(FAILED(pd3dVertexBuffer->Lock(sizeof(POINTSPRITE), sizeof(POINTSPRITE) * pointNumber, (void**)&pPS, D3DLOCK_DISCARD)))
			MessageBox(NULL, TEXT("POINTSPRITE's VertexBuffer Lock Err"), NULL, MB_OK | MB_ICONSTOP);

		for(int i = 0; i < pointNumber - 1; i++)
		{
			pPS[i].Position = CVector3(0, 0, 0);
			pPS[i].PSize = size;
			pPS[i].Color = 0;
		}

		pd3dVertexBuffer->Unlock();

		// エミッターデータの生成
		EmitterData = new CEmitter();

		// テクスチャのロード
		Texture = CTexture::CreateFromFile(path, D3DX_FILTER_NONE);
		// シェーダーを設定
		Shader = CShaderParticle::CreateShader();
	}

	CPointSprite::~CPointSprite()
	{
		SAFE_RELEASE(pd3dVertexBuffer);
		Texture.reset();

		// エミッターデータの解放
		SAFE_DELETE(EmitterData);
		
	}

	CPointSpriteSP CPointSprite::CreatePointSprite(const std::string& path, const WORD numPoint, const float size)
	{
		return CPointSpriteSP(new CPointSprite(path, numPoint, size));
	}

	void CPointSprite::Render(const CCamera* camera)
	{
		// 描画するか否か
		if(!isRender)
			return;

		// ワールド行列設定
		CMatrix SclMtx, RotMtx, PosMtx, WldMtx, WVPMtx;
		// 拡縮
		D3DXMatrixScaling(&SclMtx, Scale.x, Scale.y, Scale.z);
		// 回転 : switch-case…クォータニオンか回転行列かXYZ指定か
		switch(CurrentRotateType)
		{
		case ROTATE_TYPE::QUATERNION :
			D3DXMatrixRotationQuaternion(&RotMtx, &qRotation);
			break;

		case ROTATE_TYPE::MATRIX :
			mRotationWorld = mRotationX * mRotationY * mRotationZ;
			RotMtx = mRotationWorld;
			break;

		case ROTATE_TYPE::XYZ :
			D3DXMatrixRotationYawPitchRoll(&RotMtx, vRotation.y, vRotation.x, vRotation.z);
			break;
		}
		// 位置
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		// シェーダを使用する場合カメラのビュー行列(0)、プロジェクション行列(1)をワールド行列に合成
		WldMtx = SclMtx * RotMtx * PosMtx;
		WVPMtx = WldMtx * camera->getMatrix(CViewBehavior::VIEW) * camera->getMatrix(CViewBehavior::PROJECTION);
		// カメラの座標をシェーダに使用するための行列変換
		CMatrix CamMtx = WldMtx * camera->getMatrix(CViewBehavior::VIEW);
		D3DXMatrixInverse(&CamMtx, NULL, &CamMtx);
		CVector4 EyePos = CVector4(
			camera->getEye().x, 
			camera->getEye().y, 
			camera->getEye().z, 
			1
			);
		EyePos.Transform(CamMtx);
		D3DXVec4Normalize((D3DXVECTOR4*)&EyePos, (D3DXVECTOR4*)&EyePos);
		// シェーダー設定
		Shader->setTechnique();
		// シェーダーにワールド*ビュー*プロジェクション行列に渡す
		Shader->setWVPMatrix(WVPMtx);
		// シェーダー特有の値渡し
		Shader->applyEffect(RotMtx, EyePos);

		HRESULT hr;
		// 描画処理
		// シェーダーにカラーを渡す
		Shader->setColor(vColorRGBA);
		// テクスチャを渡す
		if(NULL != Texture)
			Shader->setTexture(Texture->getpd3dTexture());
		// シェーダーの使用開始
		Shader->BeginShader();
		// シェーダーのパス開始
		Shader->BeginPass(isAddBlend);
		// パーティクルの使用を有効にする
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
		// ポイントの最小サイズ
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, 0);
		// Zバッファ(パーティクルのみ、Z値の書き込みを行わない)
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// 頂点の設定
		CGraphicsManager::pd3dDevice->SetStreamSource(0, pd3dVertexBuffer, 0, sizeof(POINTSPRITE));
		CGraphicsManager::pd3dDevice->SetFVF(POINTSPRITE::FVF);
		// カリングを設定
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_CULLMODE, d3dCull);

		if(SUCCEEDED(CGraphicsManager::pd3dDevice->BeginScene()))
		{
			CGraphicsManager::pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, pointNumber);
			V(CGraphicsManager::pd3dDevice->EndScene());
		}
		// パス終了
		Shader->EndPass();
		// Zバッファ設定のリセット
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		// シェーダー終了
		Shader->EndShader();
	}

}