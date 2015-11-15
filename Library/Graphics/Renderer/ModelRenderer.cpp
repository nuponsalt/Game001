#include "DXUT.h"
#include "ModelRenderer.h"
#include "../GraphicsManager.h"

#include "../Shader/Shader.h"
#include "../View/Camera.h"

namespace KMT {

	ModelRenderer::ModelRenderer() { }

	void ModelRenderer::LoadFromX(const std::string &path)
	{
		// X �t�@�C�� �ƃe�N�X�`���̃��[�h
		_mesh = Mesh::CreateFromX(path);
		// �}�e���A�����̎擾
		D3DXMATERIAL *materials = (D3DXMATERIAL*)(_mesh->GetMaterialBuffer()->GetBufferPointer());
		// �e�N�X�`���̃��[�h
		CTextureSP texture;
		for(size_t i = 0; i < _mesh->GetMaterialNumber(); i++){
			// ����̕����Ńe�N�X�`�������݂��Ȃ��ꍇ
			if(NULL == materials[i].pTextureFilename){
				texture = NULL;
				_textures.push_back(texture);
				D3DCOLORVALUE value = materials[i].MatD3D.Diffuse;
				// �}�e���A������F�����擾
				D3DXVECTOR4 color = D3DXVECTOR4(value.r, value.g, value.b, value.a);
				_diffuseColors.push_back(color);
				continue;
			}
			_diffuseColors.push_back(D3DXVECTOR4(0,0,0,0));

			//---------------------------------------------------
			// �e�N�X�`���̃p�X�������I�ɐ���

			// �t�@�C���p�X�̑O�������i�[����
			std::string texturePath;
			// X�t�@�C���̃p�X����K�v�������������o��

			// "/" "\\"���������p�X�̋�؂�̍Ō�̕�������������
			// X�t�@�C���ƃe�N�X�`���͓����t�H���_�ɂ���Ƃ��A
			// X�t�@�C���̂���t�H���_�̃p�X�܂ł𔲂����

			// �p�X�̍Ō��"/"������
			std::size_t index = path.find_last_of("/");
			if(index != std::string::npos)
			{
				texturePath = path.substr(0, index + 1);
			}
			// �Y���Ȃ��Ȃ�"\\"�ōČ���
			else
			{
				index = path.find_last_of("\\");
				if(index != std::string::npos)
				{
					// �p�X�̋�؂肪"\\"�̂Ƃ���"\\"�̕������͂���������"/"������
					texturePath = path.substr(0, index);
					texturePath += "/";
				}
			}

			//------------------------------------------------------------------
			// X�t�@�C���ɋL�q����Ă���e�N�X�`���̃p�X�̍Ō�̕��������𔲂��o���O�����ɒǉ�
			std::string stringBuffer;
			stringBuffer = materials[i].pTextureFilename;
			// �p�X�̍Ō��"/"������
			index = stringBuffer.find_last_of("/");
			if(index != std::string::npos)
			{
				std::string stringBuffer2;
				stringBuffer2 = stringBuffer.substr(index + 1);
				texturePath += stringBuffer2;
			}
			// �Y���Ȃ��Ȃ�"\\"�ōČ���
			else{
				index = stringBuffer.find_last_of("\\");
				if(index != std::string::npos)
				{
					std::string stringBuffer2;
					stringBuffer2 = stringBuffer.substr(index + 1);
					texturePath += stringBuffer2;
				}
				// �Y���Ȃ��Ȃ炻�̂܂ܒǉ�
				else{
					texturePath += stringBuffer;
				}
			}
			// �Ǝ��e�N�X�`���N���X�Ƃ��Đ���
			texture = CTexture::CreateFromFile(texturePath, D3DX_DEFAULT);

			_textures.push_back(texture);
		}
	}

	GRendererSP ModelRenderer::CreateFromX(const std::string &path, const ShaderSP &shader)
	{
		GRendererSP object(new ModelRenderer());
		// �V�F�[�_�[�ݒ�
		object->SetShader(shader);
		object->LoadFromX(path);

		return GRendererSP(object);
	}

	void ModelRenderer::Render(const CCamera* camera)
	{
		if(!_renders)
			return;
		// ���[���h�s��ݒ�
		CMatrix SclMtx, RotMtx, PosMtx, WldMtx, WVPMtx;
		// �g�k
		D3DXMatrixScaling(&SclMtx, Scale.x, Scale.y, Scale.z);
		// ��] : switch-case�c�N�H�[�^�j�I������]�s��XYZ�w�肩
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
		// �ʒu
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);
		// �f�o�b�O�p
		//GraphicsManager::_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
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
			D3DXVECTOR4 color = D3DXVECTOR4(1.0,1.0,1.0,1.0);
			// �i�p�[�c�ɑΉ�����e�N�X�`����ݒ�
			// �V�F�[�_�Ƀe�N�X�`����n��
			if(NULL != _textures[i])
			{
				LPDIRECT3DTEXTURE9 texture = _textures[i]->getpd3dTexture();
				// �V�F�[�_�ɃJ���[��n��
				_shader->SetColor(_colorRGBA);
				_shader->SetTexture(texture);
			}else
				_shader->SetColor(color);
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