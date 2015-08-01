#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Vertex.h"

namespace KMT {

	class CMesh ;
	typedef std::shared_ptr<CMesh> CMeshSP;
	typedef std::weak_ptr<CMesh> CMeshWP;

	class CMesh
	{
	public :
		// デストラクタ
		~CMesh() ;
		// 空生成
		static CMeshSP Create(const std::string& _path);
		// Xファイルを読み込んで生成
		static CMeshSP CreateFromX(const std::string& _path);
		// 破棄
		static void Destroy(std::string _name = "all");
		// 取得
		inline LPD3DXMESH getpd3dMesh() { return pd3dMesh; }
		inline LPD3DXBUFFER getpd3dMaterialBuffer() { return pd3dMeterialBuffer; }
		inline DWORD getMaterialNum() const { return MaterialNumber; }
		// 設定
		inline void setpd3dMesh(LPD3DXMESH const _p) { pd3dMesh = _p; }
		inline void setVertexNum(const WORD _num) { VertexNumber = _num; }
		inline void setVertexIndexNum(const WORD _num) { IndexNumber = _num; }
		void setpd3dMeshVertexIndexBuffer(const WORD* _idx);
		void setpd3dMeshVertexBuffer(const CVertex* _info, const size_t _memsize);

	private :
		// パス
		std::string FilePath;
		// メッシュ
		LPD3DXMESH pd3dMesh;
		// マテリアルバッファー
		LPD3DXBUFFER pd3dMeterialBuffer;
		// 頂点数
		WORD VertexNumber;
		// 頂点インデックス
		BYTE* pIndex;
		// 頂点インデックス数
		WORD IndexNumber;
		// 面数
		WORD PrimitiveNumber;
		// マテリアルの数
		DWORD MaterialNumber;
		// 頂点情報
		CVertex* pVertex;
		// ハッシュマップ
		static std::unordered_map<std::string, CMeshWP> Meshs;

		// コンストラクタ
		CMesh();
	
	};
}