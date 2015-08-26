#include "DXUT.h"
#include "PointSprite.h"

#include "../Shader/Particle/Particle.h"
#include "../GraphicsManager.h"
#include "../View/Camera.h"
#include "../../Extension.h"

namespace KMT {

#define FRAND()	((float)rand() / 32768.0f)

	PointSprite::PointSprite(const std::string& path, const WORD pointNumber, const float size) : _pointNumber(pointNumber), _pointSize(size)
	{
		_path = path;

		_cullingState = D3DCULL_NONE;

		// 頂点バッファの作成
		if(FAILED(GraphicsManager::_device->CreateVertexBuffer(
			sizeof(POINTSPRITE) * _pointNumber, 
			D3DUSAGE_NPATCHES | D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC, 
			POINTSPRITE::FVF,
			D3DPOOL_DEFAULT,
			&_vertexBuffer,
			NULL
			)))
			MessageBox(NULL, TEXT("POINTSPRITE's VertexBuffer Create Err"), NULL, MB_OK | MB_ICONSTOP);

		// 頂点情報の初期化
		pPOINTSPRITE pointSprite;
		if(FAILED(_vertexBuffer->Lock(sizeof(POINTSPRITE), sizeof(POINTSPRITE) * _pointNumber, (void**)&pointSprite, D3DLOCK_DISCARD)))
			MessageBox(NULL, TEXT("POINTSPRITE's VertexBuffer Lock Err"), NULL, MB_OK | MB_ICONSTOP);

		for(int i = 0; i < _pointNumber - 1; i++)
		{
			pointSprite[i]._position = Vector3(0, 0, 0);
			pointSprite[i]._size = size;
			pointSprite[i]._color = 0;
		}

		_vertexBuffer->Unlock();

		// エミッターデータの生成
		_emitterData = new Emitter();

		// テクスチャのロード
		_texture = CTexture::CreateFromFile(path, D3DX_FILTER_NONE);
		// シェーダーを設定
		_shader = ShaderParticle::Create();
	}

	PointSprite::~PointSprite()
	{
		SAFE_RELEASE(_vertexBuffer);
		_texture.reset();

		// エミッターデータの解放
		SAFE_DELETE(_emitterData);
		
	}

	PointSpriteSP PointSprite::Create(const std::string& path, const WORD pointNumber, const float size)
	{
		return PointSpriteSP(new PointSprite(path, pointNumber, size));
	}

	void PointSprite::Render(const CCamera* camera)
	{
		// 描画するか否か
		if(!_renders)
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
		Vector4 EyePos = Vector4(
			camera->getEye().x, 
			camera->getEye().y, 
			camera->getEye().z, 
			1
			);
		EyePos.Transform(CamMtx);
		D3DXVec4Normalize((D3DXVECTOR4*)&EyePos, (D3DXVECTOR4*)&EyePos);
		// シェーダー設定
		_shader->SetTechnique();
		// シェーダーにワールド*ビュー*プロジェクション行列に渡す
		_shader->SetWVPMatrix(WVPMtx);
		// シェーダー特有の値渡し
		_shader->ApplyEffect(RotMtx, EyePos);

		HRESULT hr;
		// 描画処理
		// シェーダーにカラーを渡す
		_shader->SetColor(vColorRGBA);
		// テクスチャを渡す
		if(NULL != _texture)
			_shader->SetTexture(_texture->getpd3dTexture());
		// シェーダーの使用開始
		_shader->BeginShader();
		// シェーダーのパス開始
		_shader->BeginPass(isAddBlend);
		// パーティクルの使用を有効にする
		GraphicsManager::_device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
		GraphicsManager::_device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
		// ポイントの最小サイズ
		GraphicsManager::_device->SetRenderState(D3DRS_POINTSIZE_MIN, 0);
		// Zバッファ(パーティクルのみ、Z値の書き込みを行わない)
		GraphicsManager::_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GraphicsManager::_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// 頂点の設定
		GraphicsManager::_device->SetStreamSource(0, _vertexBuffer, 0, sizeof(POINTSPRITE));
		GraphicsManager::_device->SetFVF(POINTSPRITE::FVF);
		// カリングを設定
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);

		if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
		{
			GraphicsManager::_device->DrawPrimitive(D3DPT_POINTLIST, 0, _pointNumber);
			V(GraphicsManager::_device->EndScene());
		}
		// パス終了
		_shader->EndPass();
		// Zバッファ設定のリセット
		GraphicsManager::_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		// シェーダー終了
		_shader->EndShader();
	}

}