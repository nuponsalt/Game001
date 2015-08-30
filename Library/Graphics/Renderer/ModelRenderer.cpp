#include "DXUT.h"
#include "ModelRenderer.h"
#include "../GraphicsManager.h"

#include "../Shader/Shader.h"
#include "../View/Camera.h"

namespace KMT {

	ModelRenderer::ModelRenderer() { }

	void ModelRenderer::LoadFromX(const std::string &path)
	{
		// X ファイル とテクスチャのロード
		_mesh = CMesh::CreateFromX(path);
		// マテリアル情報の取得
		D3DXMATERIAL *materials = (D3DXMATERIAL*)(_mesh->getpd3dMaterialBuffer()->GetBufferPointer());
		// テクスチャのロード
		std::stringstream ss;
		WCHAR wc_buff[255] = { 0 };
		CTextureSP texture;
		for(size_t i = 0; i < _mesh->getMaterialNum(); i++){
			memset(wc_buff, 0, sizeof(WCHAR)*255);
			ss.str("");
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
			std::string frontPath;
			// Xファイルのパスから必要部分だけ抜き出す

			// "/" "\\"を検索しパスの区切りの最後の部分を検索する
			// Xファイルとテクスチャは同じフォルダにあるとし、
			// Xファイルのあるフォルダのパスまでを抜き取る

			// パスの最後の"/"を検索
			std::size_t index = path.find_last_of("/");
			if(index != std::string::npos)
			{
				frontPath = path.substr(0, index + 1);
			}
			// 該当なしなら"\\"で再検索
			else
			{
				index = path.find_last_of("\\");
				if(index != std::string::npos)
				{
					// パスの区切りが"\\"のときは"\\"の部分をはずしかわりに"/"をつける
					frontPath = path.substr(0, index);
					frontPath += "/";
				}
			}

			//------------------------------------------------------------------
			// Xファイルに記述されているテクスチャのパスの最後の部分だけを抜き出し前部分に追加
			std::string temp;
			temp = materials[i].pTextureFilename;
			// パスの最後の"/"を検索
			index = temp.find_last_of("/");
			if(index != std::string::npos)
			{
				std::string str1;
				str1 = temp.substr(index + 1);
				frontPath += str1;
			}
			// 該当なしなら"\\"で再検索
			else{
				index = temp.find_last_of("\\");
				if(index != std::string::npos)
				{
					std::string str1;
					str1 = temp.substr(index + 1);
					frontPath += str1;
				}
				// 該当なしならそのまま追加
				else{
					frontPath += temp;
				}
			}
			// 独自テクスチャクラスとして生成
			texture = CTexture::CreateFromFile(frontPath, D3DX_DEFAULT);

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

	void ModelRenderer::Render(const CCamera* camera)
	{
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
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);
		// デバッグ用
		//GraphicsManager::_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
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
		// シェーダ設定
		_shader->SetTechnique();
		// シェーダにワールド * ビュー * プロジェクション行列を渡す
		_shader->SetWVPMatrix(WVPMtx);
		// シェーダー特有の値の設定
		_shader->ApplyEffect(RotMtx, EyePos);
		
		HRESULT hr;
		// 3D モデルのパーツ分ループして描画
		for(size_t i = 0 ; i < _mesh->getMaterialNum(); i++)
		{
			// テクスチャが存在しない場合のカラー
			D3DXVECTOR4 color = D3DXVECTOR4(1.0,1.0,1.0,1.0);
			// 格パーツに対応するテクスチャを設定
			// シェーダにテクスチャを渡す
			if(NULL != _textures[i])
			{
				LPDIRECT3DTEXTURE9 texture = _textures[i]->getpd3dTexture();
				// シェーダにカラーを渡す
				_shader->SetColor(vColorRGBA);
				_shader->SetTexture(texture);
			}else
				_shader->SetColor(color);
			// シェーダの使用開始
			_shader->BeginShader();
			// シェーダのパス設定
			_shader->BeginPass(isAddBlend);
			// パーツの描画	
			if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
			{
				_mesh->getpd3dMesh()->DrawSubset(i); 
				V(GraphicsManager::_device->EndScene());
			}
			// パス終了
			_shader->EndPass();
			// シェーダ終了
			_shader->EndShader();
		}
	}
	
}