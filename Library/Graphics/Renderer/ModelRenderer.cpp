#include "DXUT.h"
#include "ModelRenderer.h"
#include "../GraphicsManager.h"

#include "../Shader/Shader.h"
#include "../View/Camera.h"

namespace KMT {

	CModelRenderer::CModelRenderer() { }

	void CModelRenderer::LoadFromX(const std::string &_path)
	{
		//-------------------------------------------------------------------------
		// X ファイル とテクスチャのロード
		Mesh = CMesh::CreateFromX(_path);
		// マテリアル情報の取得
		D3DXMATERIAL *p_materials = (D3DXMATERIAL*)(Mesh->getpd3dMaterialBuffer()->GetBufferPointer());
		// テクスチャのロード
		std::stringstream ss;
		WCHAR wc_buff[255] = { 0 };
		CTextureSP tex;
		for(size_t i = 0; i < Mesh->getMaterialNum(); i++){
			memset(wc_buff, 0, sizeof(WCHAR)*255);
			ss.str("");
			// 特定の部分でテクスチャが存在しない場合
			if(NULL == p_materials[i].pTextureFilename){
				tex = NULL;
				Textures.push_back(tex);
				D3DCOLORVALUE v = p_materials[i].MatD3D.Diffuse;
				// マテリアルから色情報を取得
				D3DXVECTOR4 col = D3DXVECTOR4(v.r, v.g, v.b, v.a);
				DiffuseColors.push_back(col);
				continue;
			}
			//
			DiffuseColors.push_back(D3DXVECTOR4(0,0,0,0));
			// テクスチャのパスを自動的に生成
			//------------------------------------------------

			// ファイルパスの前部分を格納する
			std::string b_path;
			// Xファイルのパスから必要部分だけ抜き出す

			// "/" "\\"を検索しパスの区切りの最後の部分を検索する
			// Xファイルとテクスチャは同じフォルダにあるとし、
			// Xファイルのあるフォルダのパスまでを抜き取る

			// パスの最後の"/"を検索
			std::size_t index = _path.find_last_of("/");
			if(index != std::string::npos)
			{
				b_path = _path.substr(0, index + 1);
			}
			// 該当なしなら"\\"で再検索
			else
			{
				index = _path.find_last_of("\\");
				if(index != std::string::npos)
				{
					// パスの区切りが"\\"のときは"\\"の部分をはずしかわりに"/"をつける
					b_path = _path.substr(0, index);
					b_path += "/";
				}
			}

			// Xファイルに記述されているテクスチャのパスの最後の部分だけを抜き出し前部分に追加
			//----------------------------------------------------
			std::string temp_str;
			temp_str = p_materials[i].pTextureFilename;
			// パスの最後の"/"を検索
			index = temp_str.find_last_of("/");
			if(index != std::string::npos)
			{
				std::string str1;
				str1 = temp_str.substr(index + 1);
				b_path += str1;
			}
			// 該当なしなら"\\"で再検索
			else{
				index = temp_str.find_last_of("\\");
				if(index != std::string::npos)
				{
					std::string str1;
					str1 = temp_str.substr(index + 1);
					b_path += str1;
				}
				// 該当なしならそのまま追加
				else{
					b_path += temp_str;
				}
			}
			// 独自テクスチャクラスとして生成
			tex = CTexture::CreateFromFile(b_path, D3DX_DEFAULT);

			Textures.push_back(tex);
		}
	}

	CGRendererSP CModelRenderer::CreateFromX(const std::string &_path, const CShaderSP &_shader)
	{
		CGRendererSP pgraph(new CModelRenderer());
		// シェーダー設定
		pgraph->setShader(_shader);
		pgraph->LoadFromX(_path);

		return CGRendererSP(pgraph);
	}

	void CModelRenderer::Render(const CCamera* camera)
	{
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
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_CULLMODE, d3dCull);
		// デバッグ用
		//CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
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
		Shader->setTechnique();
		// シェーダにワールド * ビュー * プロジェクション行列を渡す
		Shader->setWVPMatrix(WVPMtx);
		// シェーダー特有の値の設定
		Shader->applyEffect(RotMtx, EyePos);
		
		HRESULT hr;
		// 3D モデルのパーツ分ループして描画
		for(size_t i = 0 ; i < Mesh->getMaterialNum(); i++)
		{
			// テクスチャが存在しない場合のカラー
			D3DXVECTOR4 vec4 = D3DXVECTOR4(1.0,1.0,1.0,1.0);
			// 格パーツに対応するテクスチャを設定
			// シェーダにテクスチャを渡す
			if(NULL != Textures[i])
			{
				LPDIRECT3DTEXTURE9 p_tex = Textures[i]->getpd3dTexture();
				// シェーダにカラーを渡す
				Shader->setColor(vColorRGBA);
				Shader->setTexture(p_tex);
			} else 
			{
				Shader->setColor(vec4);
			}
			// シェーダの使用開始
			Shader->BeginShader();
			// シェーダのパス設定
			Shader->BeginPass(isAddBlend);
			// パーツの描画	
			if(SUCCEEDED(CGraphicsManager::pd3dDevice->BeginScene()))
			{
				Mesh->getpd3dMesh()->DrawSubset(i); 
				V(CGraphicsManager::pd3dDevice->EndScene());
			}
			// パス終了
			Shader->EndPass();
			// シェーダ終了
			Shader->EndShader();
		}
	}
	
}