﻿#include "DXUT.h"
#include "ModelRenderer.h"
#include "../GraphicsManager.h"

#include "../Shader/Shader.h"
#include "../View/Camera.h"

namespace KMT {

	ModelRenderer::ModelRenderer() { }

	void ModelRenderer::LoadFromX(const std::string &path)
	{
		// X ファイル とテクスチャのロード
		_mesh = Mesh::CreateFromX(path);
		// マテリアル情報の取得
		D3DXMATERIAL *materials = (D3DXMATERIAL*)(_mesh->GetMaterialBuffer()->GetBufferPointer());
		// テクスチャのロード
		TextureSP texture;
		for(size_t i = 0; i < _mesh->GetMaterialNumber(); i++){
			// 特定の部分でテクスチャが存在しない場合
			if(NULL == materials[i].pTextureFilename){
				texture = NULL;
				_textures.push_back(texture);
				D3DCOLORVALUE value = materials[i].MatD3D.Diffuse;
				// マテリアルから色情報を取得
				D3DXVECTOR4 color = D3DXVECTOR4(value.r, value.g, value.b, value.a);
				_diffuseColors.push_back(color);
				continue;
			}
			_diffuseColors.push_back(D3DXVECTOR4(0,0,0,0));

			//---------------------------------------------------
			// テクスチャのパスを自動的に生成

			// ファイルパスの前部分を格納する
			std::string texturePath;
			// Xファイルのパスから必要部分だけ抜き出す

			// "/" "\\"を検索しパスの区切りの最後の部分を検索する
			// Xファイルとテクスチャは同じフォルダにあるとし、
			// Xファイルのあるフォルダのパスまでを抜き取る

			// パスの最後の"/"を検索
			std::size_t index = path.find_last_of("/");
			if(index != std::string::npos)
			{
				texturePath = path.substr(0, index + 1);
			}
			// 該当なしなら"\\"で再検索
			else
			{
				index = path.find_last_of("\\");
				if(index != std::string::npos)
				{
					// パスの区切りが"\\"のときは"\\"の部分をはずしかわりに"/"をつける
					texturePath = path.substr(0, index);
					texturePath += "/";
				}
			}

			//------------------------------------------------------------------
			// Xファイルに記述されているテクスチャのパスの最後の部分だけを抜き出し前部分に追加
			std::string stringBuffer;
			stringBuffer = materials[i].pTextureFilename;
			// パスの最後の"/"を検索
			index = stringBuffer.find_last_of("/");
			if(index != std::string::npos)
			{
				std::string stringBuffer2;
				stringBuffer2 = stringBuffer.substr(index + 1);
				texturePath += stringBuffer2;
			}
			// 該当なしなら"\\"で再検索
			else{
				index = stringBuffer.find_last_of("\\");
				if(index != std::string::npos)
				{
					std::string stringBuffer2;
					stringBuffer2 = stringBuffer.substr(index + 1);
					texturePath += stringBuffer2;
				}
				// 該当なしならそのまま追加
				else{
					texturePath += stringBuffer;
				}
			}
			// 独自テクスチャクラスとして生成
			texture = Texture::CreateFromFile(texturePath, D3DX_DEFAULT);

			_textures.push_back(texture);
		}
	}

	GRendererSP ModelRenderer::CreateFromX(const std::string &path, const ShaderSP &shader)
	{
		GRendererSP object(new ModelRenderer());
		// シェーダー設定
		object->SetShader(shader);
		object->LoadFromX(path);

		return GRendererSP(object);
	}

	void ModelRenderer::Render(const Camera* camera)
	{
		if(!_renders)
			return;
		// ワールド行列設定
		Matrix SclMtx, RotMtx, PosMtx, WldMtx, WVPMtx;
		// 拡縮
		D3DXMatrixScaling(&SclMtx, _scale.x, _scale.y, _scale.z);
		// 回転 : switch-case…クォータニオンか回転行列かXYZ指定か
		this->Evaluate();
		RotMtx = this->_worldRotationMatrix;
		// 位置
		D3DXMatrixTranslation(&PosMtx, _position.x, _position.y, _position.z);
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);
		// デバッグ用
		//GraphicsManager::_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		// シェーダを使用する場合カメラのビュー行列(0)、プロジェクション行列(1)をワールド行列に合成
		WldMtx = SclMtx * RotMtx * PosMtx;
		WVPMtx = WldMtx * camera->GetMatrix(ViewBehavior::VIEW) * camera->GetMatrix(ViewBehavior::PROJECTION);
		// カメラの座標をシェーダに使用するための行列変換
		Matrix CamMtx = WldMtx * camera->GetMatrix(ViewBehavior::VIEW);
		D3DXMatrixInverse(&CamMtx, NULL, &CamMtx);
		auto eye = camera->GetEye();
		Vector4 EyePos = Vector4(eye.x, eye.y, eye.z, 1);
		EyePos.Transform(CamMtx);
		D3DXVec4Normalize((D3DXVECTOR4*)&EyePos, (D3DXVECTOR4*)&EyePos);
		// シェーダ設定
		_shader->SetTechnique();
		// シェーダにワールド * ビュー * プロジェクション行列を渡す
		_shader->SetWVPMatrix(WVPMtx);
		// シェーダー特有の値の設定
		_shader->ApplyEffect(RotMtx, EyePos);
		
		HRESULT hr;
		// 3D モデルのパーツ分ループして描画
		for(size_t i = 0 ; i < _mesh->GetMaterialNumber(); i++)
		{
			// テクスチャが存在しない場合のカラー
			D3DXVECTOR4 color = D3DXVECTOR4(1.0,1.0,1.0,1.0);
			// 格パーツに対応するテクスチャを設定
			// シェーダにテクスチャを渡す
			if(NULL != _textures[i])
			{
				LPDIRECT3DTEXTURE9 texture = _textures[i]->GetTextureData();
				// シェーダにカラーを渡す
				_shader->SetColor(_colorRGBA);
				_shader->SetTexture(texture);
			}else
				_shader->SetColor(color);
			// シェーダの使用開始
			_shader->BeginShader();
			// シェーダのパス設定
			_shader->BeginPass(_addsBlend);
			// パーツの描画	
			if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
			{
				_mesh->GetMesh()->DrawSubset(i); 
				V(GraphicsManager::_device->EndScene());
			}
			// パス終了
			_shader->EndPass();
			// シェーダ終了
			_shader->EndShader();
		}
	}
	
}