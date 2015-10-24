#include "DXUT.h"
#include "GraphicalPlane.h"

#include "../../Shader/Shader.h"
#include "../../GraphicsManager.h"
#include "../../Mesh.h"
#include "../../View/Camera.h"

#include <sstream>

using namespace KMT;
static void GetBillBoardRotation(CVector3* billBoardPosition, CVector3* targetPosition, CMatrix* rotation )
{
	D3DXMatrixIdentity((D3DXMATRIX*)rotation);
	D3DXMatrixLookAtLH(rotation, (D3DXVECTOR3*)targetPosition, (D3DXVECTOR3*)billBoardPosition, &D3DXVECTOR3(0, 1, 0));

	rotation->_41 = 0;
	rotation->_42 = 0;
	rotation->_43 = 0;

	D3DXMatrixInverse(rotation, NULL, rotation);
}

namespace KMT
{
	// static parameters
	size_t GraphicalPlane::_createCount = 0;

	GraphicalPlane::GraphicalPlane() : _textureSize(1.0f, 1.0f, 0.0f), _previousNumber(0)
	{}

	bool GraphicalPlane::GenerateBoard(const std::string& path, const int &width, const int &height, const TextureSP &texture)
	{
		// キーネーム設定
		std::stringstream nameBuffer("");
		// ファイルパス→生成番号→幅高さの順で追加
		nameBuffer << path << ++_createCount << width << height;
		std::string name = nameBuffer.str();
		// メッシュインスタンスの生成
		_mesh = CMesh::Create(name);
		// サイズを記録
		_size.x = (float)width ;
		_size.y = (float)height ;
		//_size.z = 0 ;
		// テクスチャ指定がある場合そのサイズを取得
		if(texture != NULL)
		{
			while(UINT(_textureSize.x) < _texture->GetImageInfo().Width)
			{
				_textureSize.x *= 2;
			}
			while(UINT(_textureSize.y) < _texture->GetImageInfo().Height)
			{
				_textureSize.y *= 2;
			}
		}
		// シェーダー設定
		_shader = ShaderNormal::Create();
		// メッシュを生成する
		LPD3DXMESH mesh;
		if (FAILED(D3DXCreateMeshFVF(2, 4, D3DXMESH_MANAGED, Vertex::FVF, GraphicsManager::_device, &mesh)))
			return false;

		//頂点データの作成
		Vertex* vertex;
		mesh->LockVertexBuffer(0, (void**)&vertex);
		for (int y = 0 ; y < 2 ; y++) {
			for (int x = 0 ; x < 2 ; x++) {
				float x1 = (float)(x * width - ((float)width / 2));
				float y1 = (float)(y * height - ((float)height / 2));
				int index = y * 2 + x;
				vertex[index]._position.x = x1;
				vertex[index]._position.y = y1;
				vertex[index]._position.z = 0;
				vertex[index]._normal.x = 0;
				vertex[index]._normal.y = 0;
				vertex[index]._normal.z = 1;
				if( texture == NULL )
				{
					vertex[index]._UV.x = (float)x * 1.0f;
					vertex[index]._UV.y = 1.0f - ((float)y * 1.0f);
				}
			}
		}
		if(texture)
		{
			vertex[0]._UV.x = (float)_rects[_number].left / _texture->GetImageInfo().Width;
			vertex[0]._UV.y = (float)_rects[_number].bottom / _texture->GetImageInfo().Height;
			vertex[1]._UV.x = (float)_rects[_number].right / _texture->GetImageInfo().Width;
			vertex[1]._UV.y = (float)_rects[_number].bottom / _texture->GetImageInfo().Height;
			vertex[2]._UV.x = (float)_rects[_number].left / _texture->GetImageInfo().Width;
			vertex[2]._UV.y = (float)_rects[_number].top / _texture->GetImageInfo().Height;
			vertex[3]._UV.x = (float)_rects[_number].right / _texture->GetImageInfo().Width;
			vertex[3]._UV.y = (float)_rects[_number].top / _texture->GetImageInfo().Height;
		}
		mesh->UnlockVertexBuffer();
		//インデックスデータの作成
		WORD *index;
		mesh->LockIndexBuffer(0, (void **)&index);
		index[0] = 0;
		index[1] = 2;
		index[2] = 1;
		index[3] = 1;
		index[4] = 2;
		index[5] = 3;
		mesh->UnlockIndexBuffer();
	    _mesh->setpd3dMesh(mesh);

		return true;
	}

	GraphicalPlaneSP GraphicalPlane::Create(const int &width, const int &height, const TextureSP &texture)
	{
		GraphicalPlane *object = new GraphicalPlane();
		++_createCount;
		std::stringstream nameBuffer;
		nameBuffer.str("");
		nameBuffer << _createCount;
		std::string name = nameBuffer.str().c_str();

		object->GenerateBoard(name, width, height, texture);
		return GraphicalPlaneSP(object);
	}

	void GraphicalPlane::LoadTexture(const std::string &path, const int &divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		// リソースからテクスチャを生成
		LoadTextureAndAnimation(path, divisionX, divisionY, D3DX_DEFAULT);
		// イメージサイズに合わせて板ポリゴンを生成
		(sizeX == 0 && sizeY == 0) ? GenerateBoard(path, (int)_imageSize.x, (int)_imageSize.y, _texture) : GenerateBoard(_path, sizeX, sizeY, _texture);
		// テクスチャーを挿入
		SetTexture(_texture);
	}

	void GraphicalPlane::LoadTexture (const TextureSP& texture,const int& divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		_texture = texture;
		(sizeX == 0 && sizeY == 0) ? GenerateBoard(texture->GetFilePath(), (int)_imageSize.x, (int)_imageSize.y, _texture) : GenerateBoard(texture->GetFilePath(), sizeX, sizeY, _texture);
		SetTexture(_texture);
	}

	GraphicalPlaneSP GraphicalPlane::CreateFromTexture(const std::string &path, const int &divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		// イメージサイズに合わせて板ポリを生成
		GraphicalPlane *object = new GraphicalPlane();
		// テクスチャ生成
		object->LoadTexture(path, divisionX, divisionY, sizeX, sizeY);
		// 作成したオブジェクトを返す
		return GraphicalPlaneSP(object);
	}

	GraphicalPlaneSP GraphicalPlane::CreateFromTexture(const TextureSP &texture, const int &divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		GraphicalPlane* object = new GraphicalPlane();
		object->LoadTexture(texture, divisionX, divisionY, sizeX, sizeY);

		return GraphicalPlaneSP(object);
	}

	void GraphicalPlane::Render(const CCamera* camera) 
	{
		// 描画しないならここで関数終了
		if(!_renders)
			return;
		// 分割読み込みした場合の画像範囲選択
		if(_previousNumber != _number)
		{
			Vertex* vertex;
			_mesh->getpd3dMesh()->LockVertexBuffer( 0, (void**)&vertex );
			vertex[0]._UV.x = (float)_rects[_number].left		/	_textures[0]->GetImageInfo().Width;
			vertex[0]._UV.y = (float)_rects[_number].bottom	/	_textures[0]->GetImageInfo().Height;
			vertex[1]._UV.x = (float)_rects[_number].right	/	_textures[0]->GetImageInfo().Width;
			vertex[1]._UV.y = (float)_rects[_number].bottom	/	_textures[0]->GetImageInfo().Height;
			vertex[2]._UV.x = (float)_rects[_number].left		/	_textures[0]->GetImageInfo().Width;
			vertex[2]._UV.y = (float)_rects[_number].top		/	_textures[0]->GetImageInfo().Height;
			vertex[3]._UV.x = (float)_rects[_number].right	/	_textures[0]->GetImageInfo().Width;
			vertex[3]._UV.y = (float)_rects[_number].top		/	_textures[0]->GetImageInfo().Height;
			_mesh->getpd3dMesh()->UnlockIndexBuffer();
			_previousNumber = _number;
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
		if(_isBillBoard)
		{
			CVector3 cameraPosition = camera->getEye() ;
			GetBillBoardRotation(&Position, &cameraPosition, &RotMtx);
		}
		// 位置
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		// カリングを設定
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);
		// デバッグ用
		//GraphicsManager::_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
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
			D3DXVECTOR4 vec4 = D3DXVECTOR4(1.0,1.0,1.0,1.0);
			// 格パーツに対応するテクスチャを設定
			// シェーダにテクスチャを渡す
			if(NULL != _textures[i])
			{
				LPDIRECT3DTEXTURE9 texture = _textures[i]->GetTextureData();
				// シェーダにカラーを渡す
				_shader->SetColor(vColorRGBA);
				_shader->SetTexture(texture);
			}else
				_shader->SetColor(vec4);

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