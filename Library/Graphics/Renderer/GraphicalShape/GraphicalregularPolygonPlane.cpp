#include "DXUT.h"
#include "GraphicalRegularPolygonPlane.h"

#include "../../GraphicsManager.h"
#include "../../Shader/Shader.h"

namespace KMT {

	CGraphicalRegularPolygonPlane::CGraphicalRegularPolygonPlane() : TextureSize(1.0f, 1.0f, 0.0f)
	{}

	//CGRendererSP CGraphicalRegularPolygonPlane::Create(const int &_radius, const int &_facenum, 
	//	const FACE_GENERATE_TYPE &_type, const CTextureSP &_texture) 
	//{
	//	CGraphicalRegularPolygonPlane *_obj;
	//	return CGRendererSP(_obj);
	//}

	//bool CGraphicalRegularPolygonPlane::GenerateRegularPolygonBoard(const int &_radius, const int &_facenum, 
	//	const FACE_GENERATE_TYPE &_type, const CTextureSP &_texture) 
	//{
	//	// キーネーム設定
	//	std::stringstream ss("");
	//	ss << _radius << _facenum;
	//	std::string name = ss.str();
	//	// メッシュインスタンスの生成
	//	Mesh = CMesh::Create(name);
	//	// サイズを記憶
	//	Size.x = _radius * 2.0f;
	//	Size.y = _radius * 2.0f;
	//	Size.z = 0.0f;
	//	// テクスチャ指定がある場合テクスチャーのサイズを取得
	//	if(_texture != NULL)
	//	{
	//		while(UINT(TextureSize.x) < _texture->getd3dImageInfo().Width)
	//		{
	//			TextureSize.x *= 2;
	//		}
	//		while(UINT(TextureSize.y) < _texture->getd3dImageInfo().Height)
	//		{
	//			TextureSize.y *= 2;
	//		}
	//	}
	//	// シェーダー設定
	//	Shader = CShader::Create();
	//	// メッシュ情報読み込み
	//	LPD3DXMESH _mesh;
	//	// 頂点数は面数+中心頂点でできる
	//	//if (FAILED(D3DXCreateMeshFVF(_facenum, (_type == )(_facenum+1), D3DXMESH_MANAGED, CVertex::FVF, CGraphicsManager::pd3dDevice, &_mesh)))
	//	//{
	//	//	return false;
	//	//}
	//	// 定数
	//	static const float TWO = 2.0f;
	//	// 頂点データの作成
	//	CVertex* pV;
	//	_mesh->LockVertexBuffer(0, (void**)&pV);
	//	//インデックスデータの作成
	//	WORD *pIndex;
	//	_mesh->LockIndexBuffer( 0, (void **)&pIndex );
	//	pV[0].position = CVector3();
	//	pV[0].normal = CVector3(0.0f, 0.0f, 1.0f);
	//	for (int i = 0; i < _facenum; i++) 
	//	{
	//		if (_type == POLYNUM_VTX)
	//		{
	//			float x = _radius * cos(TWO * D3DX_PI * ((float)i / (float)_facenum) - D3DXToRadian(90));
	//			float y = _radius * sin(TWO * D3DX_PI * ((float)i / (float)_facenum) - D3DXToRadian(90));
	//			pV[i + 1].position = CVector3(x, y, 0);
	//			pV[i + 1].normal = CVector3(0, 0, 1);
	//			if(_texture == NULL)
	//			{
	//				//pV[i].uv.x = (float)x * 1.0f ;
	//				//pV[i].uv.y = 1.0f -((float)y * 1.0f) ;
	//			}
	//			pIndex[i * 3]  = 0;
	//			pIndex[i * 3 + 1] = (i !=_facenum - 1) ? i + 2 : 1;
	//			pIndex[i * 3 + 2] = i + 1;
	//			// 処理を抜ける
	//			continue;
	//		}
	//	}
	//	_mesh->UnlockVertexBuffer();
	//	_mesh->UnlockIndexBuffer();
	//	// メッシュに設定
	//	Mesh->setpd3dMesh( _mesh ) ;
	//	// 作成成功
	//	return true;
	//}
}