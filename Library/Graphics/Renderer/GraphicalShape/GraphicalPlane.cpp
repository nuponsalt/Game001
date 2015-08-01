#include "DXUT.h"
#include "GraphicalPlane.h"

#include "../../Shader/Shader.h"
#include "../../GraphicsManager.h"
#include "../../Mesh.h"
#include "../../View/Camera.h"

#include <sstream>

using namespace KMT;
static void getBillBoardRotation(CVector3* _BillPos, CVector3* _TargetPos, CMatrix* _Rot )
{
	D3DXMatrixIdentity((D3DXMATRIX*)_Rot);
	D3DXMatrixLookAtLH(_Rot, (D3DXVECTOR3*)_TargetPos, (D3DXVECTOR3*)_BillPos, &D3DXVECTOR3(0, 1, 0));

	_Rot->_41 = 0;
	_Rot->_42 = 0;
	_Rot->_43 = 0;

	D3DXMatrixInverse(_Rot, NULL, _Rot);
}

namespace KMT
{
	// static parameters
	size_t CGraphicalPlane::CreateCount = 0;

	CGraphicalPlane::CGraphicalPlane() : TextureSize(1.0f, 1.0f, 0.0f), PreviousNumber(0)
	{}

	bool CGraphicalPlane::GenerateBoard(const std::string& _path, const int &_width, const int &_height, const CTextureSP &_texture)
	{
		// キーネーム設定
		std::stringstream ss("");
		// ファイルパス→生成番号→幅高さの順で追加
		ss << _path << ++CreateCount << _width << _height;
		std::string name = ss.str();
		// メッシュインスタンスの生成
		Mesh = CMesh::Create(name);
		// サイズを記録
		Size.x = (float)_width ;
		Size.y = (float)_height ;
		//Size.z = 0 ;
		// テクスチャ指定がある場合そのサイズを取得
		if(_texture != NULL)
		{
			while(UINT(TextureSize.x) < Texture->getd3dImageInfo().Width)
			{
				TextureSize.x *= 2;
			}
			while(UINT(TextureSize.y) < Texture->getd3dImageInfo().Height)
			{
				TextureSize.y *= 2;
			}
		}
		// シェーダー設定
		Shader = CShaderNormal::CreateShader();
		// メッシュを生成する
		LPD3DXMESH _mesh;
		if (FAILED(D3DXCreateMeshFVF(2, 4, D3DXMESH_MANAGED, CVertex::FVF, CGraphicsManager::pd3dDevice, &_mesh)))
			return false;

		//頂点データの作成
		CVertex* pV;
		_mesh->LockVertexBuffer(0, (void**)&pV);
		for (int y = 0 ; y < 2 ; y++) {
			for (int x = 0 ; x < 2 ; x++) {
				float x1 = (float)(x * _width - ((float)_width / 2));
				float y1 = (float)(y * _height - ((float)_height / 2));
				int idx = y * 2 + x;
				pV[idx].Position.x = x1;
				pV[idx].Position.y = y1;
				pV[idx].Position.z = 0;
				pV[idx].Normal.x = 0;
				pV[idx].Normal.y = 0;
				pV[idx].Normal.z = 1;
				if( _texture == NULL )
				{
					pV[idx].UV.x = (float)x * 1.0f;
					pV[idx].UV.y = 1.0f - ((float)y * 1.0f);
				}
			}
		}
		if(_texture)
		{
			pV[0].UV.x = (float)Rects[Number].left / Texture->getd3dImageInfo().Width;
			pV[0].UV.y = (float)Rects[Number].bottom / Texture->getd3dImageInfo().Height;
			pV[1].UV.x = (float)Rects[Number].right / Texture->getd3dImageInfo().Width;
			pV[1].UV.y = (float)Rects[Number].bottom / Texture->getd3dImageInfo().Height;
			pV[2].UV.x = (float)Rects[Number].left / Texture->getd3dImageInfo().Width;
			pV[2].UV.y = (float)Rects[Number].top / Texture->getd3dImageInfo().Height;
			pV[3].UV.x = (float)Rects[Number].right / Texture->getd3dImageInfo().Width;
			pV[3].UV.y = (float)Rects[Number].top / Texture->getd3dImageInfo().Height;
		}
		_mesh->UnlockVertexBuffer();
		//インデックスデータの作成
		WORD *pIndex;
		_mesh->LockIndexBuffer(0, (void **)&pIndex);
		pIndex[0] = 0;
		pIndex[1] = 2;
		pIndex[2] = 1;
		pIndex[3] = 1;
		pIndex[4] = 2;
		pIndex[5] = 3;
		_mesh->UnlockIndexBuffer();
	    Mesh->setpd3dMesh(_mesh);

		return true;
	}

	CGraphicalPlaneSP CGraphicalPlane::Create(const int &_width, const int &_height, const CTextureSP &_texture)
	{
		CGraphicalPlane *_obj = new CGraphicalPlane();
		++CreateCount;
		std::stringstream ss;
		ss.str("");
		ss << CreateCount;
		std::string str = ss.str().c_str();

		_obj->GenerateBoard(str, _width, _height, _texture);
		return CGraphicalPlaneSP(_obj);
	}

	void CGraphicalPlane::LoadTexture(const std::string &_path, const int &x_num, const int &y_num, const int &size_x, const int &size_y)
	{
		// リソースからテクスチャを生成
		LoadTextureAndAnimation(_path, x_num, y_num, D3DX_DEFAULT);
		// イメージサイズに合わせて板ポリゴンを生成
		(size_x == 0 && size_y == 0) ? GenerateBoard(_path, (int)ImageSize.x, (int)ImageSize.y, Texture) : GenerateBoard(_path, size_x, size_y, Texture);
		// テクスチャーを挿入
		setTexture(Texture);
	}

	void CGraphicalPlane::LoadTexture (const CTextureSP& _tex,const int& x_num, const int &y_num, const int &size_x, const int &size_y)
	{
		Texture = _tex;
		(size_x == 0 && size_y == 0) ? GenerateBoard(_tex->getFilePath(), (int)ImageSize.x, (int)ImageSize.y, Texture) : GenerateBoard(_tex->getFilePath(), size_x, size_y, Texture);
		setTexture(Texture);
	}

	CGraphicalPlaneSP CGraphicalPlane::CreateFromTexture(const std::string &_path, const int &x_num, const int &y_num, const int &size_x, const int &size_y)
	{
		// イメージサイズに合わせて板ポリを生成
		CGraphicalPlane *_obj = new CGraphicalPlane();
		// テクスチャ生成
		_obj->LoadTexture(_path, x_num, y_num, size_x, size_y);
		// 作成したオブジェクトを返す
		return CGraphicalPlaneSP(_obj);
	}

	CGraphicalPlaneSP CGraphicalPlane::CreateFromTexture(const CTextureSP &_tex, const int &x_num, const int &y_num, const int &size_x, const int &size_y)
	{
		CGraphicalPlane* _obj = new CGraphicalPlane();
		_obj->LoadTexture(_tex, x_num, y_num, size_x, size_y);
		// 
		return CGraphicalPlaneSP(_obj);
	}

	void CGraphicalPlane::Render(const CCamera* camera) 
	{
		// 描画しないならここで関数終了
		if(!isRender)
			return;
		// 分割読み込みした場合の画像範囲選択
		if(PreviousNumber != Number)
		{
			CVertex* pV;
			Mesh->getpd3dMesh()->LockVertexBuffer( 0, (void**)&pV );
			pV[0].UV.x = (float)Rects[Number].left / Textures[0]->getd3dImageInfo().Width;
			pV[0].UV.y = (float)Rects[Number].bottom / Textures[0]->getd3dImageInfo().Height;
			pV[1].UV.x = (float)Rects[Number].right / Textures[0]->getd3dImageInfo().Width;
			pV[1].UV.y = (float)Rects[Number].bottom / Textures[0]->getd3dImageInfo().Height;
			pV[2].UV.x = (float)Rects[Number].left / Textures[0]->getd3dImageInfo().Width;
			pV[2].UV.y = (float)Rects[Number].top / Textures[0]->getd3dImageInfo().Height;
			pV[3].UV.x = (float)Rects[Number].right / Textures[0]->getd3dImageInfo().Width;
			pV[3].UV.y = (float)Rects[Number].top / Textures[0]->getd3dImageInfo().Height;
			Mesh->getpd3dMesh()->UnlockIndexBuffer();
			PreviousNumber = Number;
		}
		// ワールド行列設定
		CMatrix SclMtx, RotMtx, PosMtx, WldMtx, WVPMtx;
		// 拡縮
		D3DXMatrixScaling(&SclMtx, Scale.x, Scale.y, Scale.z);
		// 回転
		// クォータニオンか回転行列かXYZ指定か
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
			D3DXMatrixRotationYawPitchRoll( &RotMtx, vRotation.y, vRotation.x, vRotation.z );
			break;
		}
		// ビルボードの場合
		if(isBillBoard)
		{
			CVector3 campos = camera->getEye() ;
			getBillBoardRotation(&Position, &campos, &RotMtx);
		}
		// 位置
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		// カリングを設定
		CGraphicsManager::pd3dDevice->SetRenderState(D3DRS_CULLMODE, d3dCull);
		// デバッグ用
		//CGraphicsManager::pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
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