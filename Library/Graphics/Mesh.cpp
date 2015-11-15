#include "DXUT.h"
#include "Mesh.h"

#include "GraphicsManager.h"
#include "../Extension.h"

namespace KMT {

	std::unordered_map<std::string, MeshWP> Mesh::_meshes;

	Mesh::Mesh() : _mesh(NULL), _meterialBuffer(NULL), _materialNumber(1), _vertexInformation(NULL)
	{}

	Mesh::~Mesh()
	{
		// �C�e���[�^�̏���
		// �t�@�C���p�X�Ō���
		std::unordered_map<std::string, MeshWP>::iterator it = _meshes.find(_path);

		if(it != _meshes.end())
		{
			_meshes.erase( it ) ;
		}
		SAFE_RELEASE(_mesh);
		SAFE_RELEASE(_meterialBuffer);
#if _DEBUG
		OutputDebugString(L"Release Mesh\n");
#endif
	}

	MeshSP Mesh::CreateEmpty(const std::string& path)
	{
		std::unordered_map<std::string, MeshWP>::iterator it = _meshes.find(path);
		// ���݂���ΕԂ�
		if(it != _meshes.end())
			return it->second.lock();
		
		MeshSP mesh = MeshSP(new Mesh);
		mesh->_path = path;
		_meshes.insert(make_pair(path, mesh));

		return mesh;
	}

	MeshSP Mesh::CreateFromX(const std::string& path)
	{
		std::unordered_map<std::string, MeshWP>::iterator it = _meshes.find(path);

		if(it != _meshes.end()) 
			return MeshSP((*it).second.lock());
		// ���݂��Ȃ���ΐV�������[�h
		MeshSP mesh = MeshSP(new Mesh);
		// �t�@�C���p�X���擾
		mesh->_path = path;
		// ������ϊ�
		WCHAR wcbuff[255] = {0};
		DXconvAnsiToWide(wcbuff, path.c_str(), 255);
		// ���b�V���̃��[�h
		D3DXLoadMeshFromX(wcbuff,
			D3DXMESH_MANAGED,
			GraphicsManager::_device,
			NULL,
			&mesh->_meterialBuffer,
			NULL,
			&mesh->_materialNumber,
			&mesh->_mesh);
		// �}�b�v�ɒǉ�
		_meshes.insert(std::make_pair(path, MeshWP(mesh)));

		return mesh ;
	}

	void Mesh::Destroy(std::string name) 
	{
		// �C�e���[�^��p��
		// �C�e���[�^���n�b�V���}�b�v�̐擪�ɃZ�b�g
		std::unordered_map<std::string, MeshWP>::iterator it = _meshes.begin();
		// �S�����
		if(name == "all"){
			while(it != _meshes.end())
			{
				(*it).second.reset();
				_meshes.erase(it++);
			}
		}
		else // �e�N�X�`�����P�������			
		{
			it = _meshes.find(name);
			if(it != _meshes.end())
			{
				(*it).second.reset();
				_meshes.erase(it);
			}
		}
	}

	void Mesh::SetMeshVertexIndexBuffer(const WORD* index)
	{
		WORD *pI;
		_mesh->LockIndexBuffer(0, (void **)&pI);
		memcpy(pI, index, sizeof(WORD) * _indexNumber);
		_mesh->UnlockIndexBuffer();
	}

	void Mesh::SetMeshVertexBuffer(const CVertex* vertexInfo, const size_t memorySize)
	{
		_mesh->LockVertexBuffer(0, (void**)&_vertexInformation);
		memcpy(_vertexInformation, vertexInfo, memorySize * _vertexNumber);
		_mesh->UnlockVertexBuffer();
	}
}