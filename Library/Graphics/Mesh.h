#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Vertex.h"

namespace KMT {

	class Mesh ;
	typedef std::shared_ptr<Mesh> MeshSP;
	typedef std::weak_ptr<Mesh> MeshWP;

	class Mesh
	{
	public :
		// デストラクタ
		~Mesh() ;
		// 空生成
		static MeshSP CreateEmpty(const std::string& path);
		// Xファイルを読み込んで生成
		static MeshSP CreateFromX(const std::string& path);
		// 破棄
		static void Destroy(std::string name = "all");
		// 取得
		inline LPD3DXMESH GetMesh() { return _mesh; }
		inline LPD3DXBUFFER GetMaterialBuffer() { return _meterialBuffer; }
		inline DWORD GetMaterialNumber() const { return _materialNumber; }
		// 設定
		inline void SetMesh(LPD3DXMESH const mesh) { _mesh = mesh; }
		inline void setVertexNum(const WORD number) { _vertexNumber = number; }
		inline void setVertexIndexNum(const WORD number) { _indexNumber = number; }
		void SetIndexBuffer(const WORD* index);
		void SetVertexBuffer(const Vertex* vertexInfo, const size_t memorySize);

	private :
		// パス
		std::string _path;
		// メッシュ
		LPD3DXMESH _mesh;
		// マテリアルバッファー
		LPD3DXBUFFER _meterialBuffer;
		// 頂点数
		WORD _vertexNumber;
		// 頂点インデックス
		BYTE* _index;
		// 頂点インデックス数
		WORD _indexNumber;
		// 面数
		WORD _faceNumber;
		// マテリアルの数
		DWORD _materialNumber;
		// 頂点情報
		Vertex* _vertexInformation;
		// ハッシュマップ
		static std::unordered_map<std::string, MeshWP> _meshes;

		// コンストラクタ
		Mesh();
	
	};
}