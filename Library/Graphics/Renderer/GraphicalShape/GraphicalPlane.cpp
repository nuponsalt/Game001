#include "DXUT.h"
#include "GraphicalPlane.h"

#include "../../Shader/Shader.h"
#include "../../GraphicsManager.h"
#include "../../Mesh.h"
#include "../../View/Camera.h"

#include <sstream>

using namespace KMT;
static void GetBillBoardRotation(Vector3* billBoardPosition, Vector3* targetPosition, CMatrix* rotation )
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

	bool GraphicalPlane::GenerateBoard(const std::string& path, const int &width, const int &height, const CTextureSP &texture)
	{
		// �L�[�l�[���ݒ�
		std::stringstream nameBuffer("");
		// �t�@�C���p�X�������ԍ����������̏��Œǉ�
		nameBuffer << path << ++_createCount << width << height;
		std::string name = nameBuffer.str();
		// ���b�V���C���X�^���X�̐���
		_mesh = Mesh::CreateEmpty(name);
		// �T�C�Y���L�^
		_size.x = (float)width ;
		_size.y = (float)height ;
		//_size.z = 0 ;
		// �e�N�X�`���w�肪����ꍇ���̃T�C�Y���擾
		if(texture != NULL)
		{
			while(UINT(_textureSize.x) < Texture->getd3dImageInfo().Width)
			{
				_textureSize.x *= 2;
			}
			while(UINT(_textureSize.y) < Texture->getd3dImageInfo().Height)
			{
				_textureSize.y *= 2;
			}
		}
		// �V�F�[�_�[�ݒ�
		_shader = ShaderNormal::Create();
		// ���b�V���𐶐�����
		LPD3DXMESH mesh;
		if (FAILED(D3DXCreateMeshFVF(2, 4, D3DXMESH_MANAGED, CVertex::FVF, GraphicsManager::_device, &mesh)))
			return false;

		//���_�f�[�^�̍쐬
		CVertex* vertex;
		mesh->LockVertexBuffer(0, (void**)&vertex);
		for (int y = 0 ; y < 2 ; y++) {
			for (int x = 0 ; x < 2 ; x++) {
				float x1 = (float)(x * width - ((float)width / 2));
				float y1 = (float)(y * height - ((float)height / 2));
				int index = y * 2 + x;
				vertex[index].Position.x = x1;
				vertex[index].Position.y = y1;
				vertex[index].Position.z = 0;
				vertex[index].Normal.x = 0;
				vertex[index].Normal.y = 0;
				vertex[index].Normal.z = 1;
				if( texture == NULL )
				{
					vertex[index].UV.x = (float)x * 1.0f;
					vertex[index].UV.y = 1.0f - ((float)y * 1.0f);
				}
			}
		}
		if(texture)
		{
			vertex[0].UV.x = (float)Rects[Number].left / Texture->getd3dImageInfo().Width;
			vertex[0].UV.y = (float)Rects[Number].bottom / Texture->getd3dImageInfo().Height;
			vertex[1].UV.x = (float)Rects[Number].right / Texture->getd3dImageInfo().Width;
			vertex[1].UV.y = (float)Rects[Number].bottom / Texture->getd3dImageInfo().Height;
			vertex[2].UV.x = (float)Rects[Number].left / Texture->getd3dImageInfo().Width;
			vertex[2].UV.y = (float)Rects[Number].top / Texture->getd3dImageInfo().Height;
			vertex[3].UV.x = (float)Rects[Number].right / Texture->getd3dImageInfo().Width;
			vertex[3].UV.y = (float)Rects[Number].top / Texture->getd3dImageInfo().Height;
		}
		mesh->UnlockVertexBuffer();
		//�C���f�b�N�X�f�[�^�̍쐬
		WORD *index;
		mesh->LockIndexBuffer(0, (void **)&index);
		index[0] = 0;
		index[1] = 2;
		index[2] = 1;
		index[3] = 1;
		index[4] = 2;
		index[5] = 3;
		mesh->UnlockIndexBuffer();
	    _mesh->SetMesh(mesh);

		return true;
	}

	GraphicalPlaneSP GraphicalPlane::Create(const int &width, const int &height, const CTextureSP &texture)
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
		// ���\�[�X����e�N�X�`���𐶐�
		LoadTextureAndAnimation(path, divisionX, divisionY, D3DX_DEFAULT);
		// �C���[�W�T�C�Y�ɍ��킹�Ĕ|���S���𐶐�
		(sizeX == 0 && sizeY == 0) ? GenerateBoard(path, (int)ImageSize.x, (int)ImageSize.y, Texture) : GenerateBoard(_path, sizeX, sizeY, Texture);
		// �e�N�X�`���[��}��
		SetTexture(Texture);
	}

	void GraphicalPlane::LoadTexture (const CTextureSP& texture,const int& divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		Texture = texture;
		(sizeX == 0 && sizeY == 0) ? GenerateBoard(texture->getFilePath(), (int)ImageSize.x, (int)ImageSize.y, Texture) : GenerateBoard(texture->getFilePath(), sizeX, sizeY, Texture);
		SetTexture(Texture);
	}

	GraphicalPlaneSP GraphicalPlane::CreateFromTexture(const std::string &path, const int &divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		// �C���[�W�T�C�Y�ɍ��킹�Ĕ|���𐶐�
		GraphicalPlane *object = new GraphicalPlane();
		// �e�N�X�`������
		object->LoadTexture(path, divisionX, divisionY, sizeX, sizeY);
		// �쐬�����I�u�W�F�N�g��Ԃ�
		return GraphicalPlaneSP(object);
	}

	GraphicalPlaneSP GraphicalPlane::CreateFromTexture(const CTextureSP &texture, const int &divisionX, const int &divisionY, const int &sizeX, const int &sizeY)
	{
		GraphicalPlane* object = new GraphicalPlane();
		object->LoadTexture(texture, divisionX, divisionY, sizeX, sizeY);

		return GraphicalPlaneSP(object);
	}

	void GraphicalPlane::Render(const CCamera* camera) 
	{
		// �`�悵�Ȃ��Ȃ炱���Ŋ֐��I��
		if(!_renders)
			return;
		// �����ǂݍ��݂����ꍇ�̉摜�͈͑I��
		if(_previousNumber != Number)
		{
			CVertex* vertex;
			_mesh->GetMesh()->LockVertexBuffer( 0, (void**)&vertex );
			vertex[0].UV.x = (float)Rects[Number].left		/	_textures[0]->getd3dImageInfo().Width;
			vertex[0].UV.y = (float)Rects[Number].bottom	/	_textures[0]->getd3dImageInfo().Height;
			vertex[1].UV.x = (float)Rects[Number].right		/	_textures[0]->getd3dImageInfo().Width;
			vertex[1].UV.y = (float)Rects[Number].bottom	/	_textures[0]->getd3dImageInfo().Height;
			vertex[2].UV.x = (float)Rects[Number].left		/	_textures[0]->getd3dImageInfo().Width;
			vertex[2].UV.y = (float)Rects[Number].top		/	_textures[0]->getd3dImageInfo().Height;
			vertex[3].UV.x = (float)Rects[Number].right		/	_textures[0]->getd3dImageInfo().Width;
			vertex[3].UV.y = (float)Rects[Number].top		/	_textures[0]->getd3dImageInfo().Height;
			_mesh->GetMesh()->UnlockIndexBuffer();
			_previousNumber = Number;
		}
		// ���[���h�s��ݒ�
		CMatrix SclMtx, RotMtx, PosMtx, WldMtx, WVPMtx;
		// �g�k
		D3DXMatrixScaling(&SclMtx, Scale.x, Scale.y, Scale.z);
		// ��]
		// �N�H�[�^�j�I������]�s��XYZ�w�肩
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
		// �r���{�[�h�̏ꍇ
		if(_isBillBoard)
		{
			Vector3 cameraPosition = camera->getEye() ;
			GetBillBoardRotation(&Position, &cameraPosition, &RotMtx);
		}
		// �ʒu
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		// �J�����O��ݒ�
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);
		// �f�o�b�O�p
		//GraphicsManager::_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		// �V�F�[�_���g�p����ꍇ�J�����̃r���[�s��(0)�A�v���W�F�N�V�����s��(1)�����[���h�s��ɍ���
		WldMtx = SclMtx * RotMtx * PosMtx;
		WVPMtx = WldMtx * camera->getMatrix(CViewBehavior::VIEW) * camera->getMatrix(CViewBehavior::PROJECTION);
		// �J�����̍��W���V�F�[�_�Ɏg�p���邽�߂̍s��ϊ�
		CMatrix CamMtx = WldMtx * camera->getMatrix(CViewBehavior::VIEW);
		D3DXMatrixInverse(&CamMtx, NULL, &CamMtx);
		Vector4 EyePos = Vector4(
			camera->getEye().x, 
			camera->getEye().y, 
			camera->getEye().z, 
			1
			);
		EyePos.Transform(CamMtx);
		D3DXVec4Normalize((D3DXVECTOR4*)&EyePos, (D3DXVECTOR4*)&EyePos);
		// �V�F�[�_�ݒ�
		_shader->SetTechnique();
		// �V�F�[�_�Ƀ��[���h * �r���[ * �v���W�F�N�V�����s���n��
		_shader->SetWVPMatrix(WVPMtx);
		// �V�F�[�_�[���L�̒l�̐ݒ�
		_shader->ApplyEffect(RotMtx, EyePos);

		HRESULT hr;
		// 3D ���f���̃p�[�c�����[�v���ĕ`��
		for(size_t i = 0 ; i < _mesh->GetMaterialNumber(); i++)
		{
			// �e�N�X�`�������݂��Ȃ��ꍇ�̃J���[
			D3DXVECTOR4 vec4 = D3DXVECTOR4(1.0,1.0,1.0,1.0);
			// �i�p�[�c�ɑΉ�����e�N�X�`����ݒ�
			// �V�F�[�_�Ƀe�N�X�`����n��
			if(NULL != _textures[i])
			{
				LPDIRECT3DTEXTURE9 texture = _textures[i]->getpd3dTexture();
				// �V�F�[�_�ɃJ���[��n��
				_shader->SetColor(_colorRGBA);
				_shader->SetTexture(texture);
			}else
				_shader->SetColor(vec4);

			// �V�F�[�_�̎g�p�J�n
			_shader->BeginShader();
			// �V�F�[�_�̃p�X�ݒ�
			_shader->BeginPass(_addsBlend);
			// �p�[�c�̕`��	
			if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
			{
				_mesh->GetMesh()->DrawSubset(i); 
				V(GraphicsManager::_device->EndScene());
			}
			// �p�X�I��
			_shader->EndPass();
			// �V�F�[�_�I��
			_shader->EndShader();
		}
	}
}