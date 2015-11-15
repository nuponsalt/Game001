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
		// �f�X�g���N�^
		~Mesh() ;
		// �󐶐�
		static MeshSP CreateEmpty(const std::string& path);
		// X�t�@�C����ǂݍ���Ő���
		static MeshSP CreateFromX(const std::string& path);
		// �j��
		static void Destroy(std::string name = "all");
		// �擾
		inline LPD3DXMESH GetMesh() { return _mesh; }
		inline LPD3DXBUFFER GetMaterialBuffer() { return _meterialBuffer; }
		inline DWORD GetMaterialNumber() const { return _materialNumber; }
		// �ݒ�
		inline void SetMesh(LPD3DXMESH const mesh) { _mesh = mesh; }
		inline void setVertexNum(const WORD number) { _vertexNumber = number; }
		inline void setVertexIndexNum(const WORD number) { _indexNumber = number; }
		void SetMeshVertexIndexBuffer(const WORD* index);
		void SetMeshVertexBuffer(const CVertex* vertexInfo, const size_t memorySize);

	private :
		// �p�X
		std::string _path;
		// ���b�V��
		LPD3DXMESH _mesh;
		// �}�e���A���o�b�t�@�[
		LPD3DXBUFFER _meterialBuffer;
		// ���_��
		WORD _vertexNumber;
		// ���_�C���f�b�N�X
		BYTE* _index;
		// ���_�C���f�b�N�X��
		WORD _indexNumber;
		// �ʐ�
		WORD _faceNumber;
		// �}�e���A���̐�
		DWORD _materialNumber;
		// ���_���
		CVertex* _vertexInformation;
		// �n�b�V���}�b�v
		static std::unordered_map<std::string, MeshWP> _meshes;

		// �R���X�g���N�^
		Mesh();
	
	};
}