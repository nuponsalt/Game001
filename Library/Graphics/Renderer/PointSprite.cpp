#include "DXUT.h"
#include "PointSprite.h"

#include "../Shader/Particle/Particle.h"
#include "../GraphicsManager.h"
#include "../View/Camera.h"
#include "../../Extension.h"

namespace KMT {

#define FRAND()	((float)rand() / 32768.0f)

	PointSprite::PointSprite(const std::string& path, const WORD pointNumber, const float size) : _pointNumber(pointNumber), _pointSize(size)
	{
		_path = path;

		_cullingState = D3DCULL_NONE;

		// ���_�o�b�t�@�̍쐬
		if(FAILED(GraphicsManager::_device->CreateVertexBuffer(
			sizeof(POINTSPRITE) * _pointNumber, 
			D3DUSAGE_NPATCHES | D3DUSAGE_POINTS | D3DUSAGE_DYNAMIC, 
			POINTSPRITE::FVF,
			D3DPOOL_DEFAULT,
			&_vertexBuffer,
			NULL
			)))
			MessageBox(NULL, TEXT("POINTSPRITE's VertexBuffer Create Err"), NULL, MB_OK | MB_ICONSTOP);

		// ���_���̏�����
		pPOINTSPRITE pointSprite;
		if(FAILED(_vertexBuffer->Lock(sizeof(POINTSPRITE), sizeof(POINTSPRITE) * _pointNumber, (void**)&pointSprite, D3DLOCK_DISCARD)))
			MessageBox(NULL, TEXT("POINTSPRITE's VertexBuffer Lock Err"), NULL, MB_OK | MB_ICONSTOP);

		for(int i = 0; i < _pointNumber - 1; i++)
		{
			pointSprite[i]._position = Vector3(0, 0, 0);
			pointSprite[i]._size = size;
			pointSprite[i]._color = 0;
		}

		_vertexBuffer->Unlock();

		// �G�~�b�^�[�f�[�^�̐���
		_emitterData = new Emitter();

		// �e�N�X�`���̃��[�h
		_texture = CTexture::CreateFromFile(path, D3DX_FILTER_NONE);
		// �V�F�[�_�[��ݒ�
		_shader = ShaderParticle::Create();
	}

	PointSprite::~PointSprite()
	{
		SAFE_RELEASE(_vertexBuffer);
		_texture.reset();

		// �G�~�b�^�[�f�[�^�̉��
		SAFE_DELETE(_emitterData);
		
	}

	PointSpriteSP PointSprite::Create(const std::string& path, const WORD pointNumber, const float size)
	{
		return PointSpriteSP(new PointSprite(path, pointNumber, size));
	}

	void PointSprite::Render(const CCamera* camera)
	{
		// �`�悷�邩�ۂ�
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
		// �V�F�[�_�[�ݒ�
		_shader->SetTechnique();
		// �V�F�[�_�[�Ƀ��[���h*�r���[*�v���W�F�N�V�����s��ɓn��
		_shader->SetWVPMatrix(WVPMtx);
		// �V�F�[�_�[���L�̒l�n��
		_shader->ApplyEffect(RotMtx, EyePos);

		HRESULT hr;
		// �`�揈��
		// �V�F�[�_�[�ɃJ���[��n��
		_shader->SetColor(vColorRGBA);
		// �e�N�X�`����n��
		if(NULL != _texture)
			_shader->SetTexture(_texture->getpd3dTexture());
		// �V�F�[�_�[�̎g�p�J�n
		_shader->BeginShader();
		// �V�F�[�_�[�̃p�X�J�n
		_shader->BeginPass(isAddBlend);
		// �p�[�e�B�N���̎g�p��L���ɂ���
		GraphicsManager::_device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
		GraphicsManager::_device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
		// �|�C���g�̍ŏ��T�C�Y
		GraphicsManager::_device->SetRenderState(D3DRS_POINTSIZE_MIN, 0);
		// Z�o�b�t�@(�p�[�e�B�N���̂݁AZ�l�̏������݂��s��Ȃ�)
		GraphicsManager::_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GraphicsManager::_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// ���_�̐ݒ�
		GraphicsManager::_device->SetStreamSource(0, _vertexBuffer, 0, sizeof(POINTSPRITE));
		GraphicsManager::_device->SetFVF(POINTSPRITE::FVF);
		// �J�����O��ݒ�
		GraphicsManager::_device->SetRenderState(D3DRS_CULLMODE, _cullingState);

		if(SUCCEEDED(GraphicsManager::_device->BeginScene()))
		{
			GraphicsManager::_device->DrawPrimitive(D3DPT_POINTLIST, 0, _pointNumber);
			V(GraphicsManager::_device->EndScene());
		}
		// �p�X�I��
		_shader->EndPass();
		// Z�o�b�t�@�ݒ�̃��Z�b�g
		GraphicsManager::_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		// �V�F�[�_�[�I��
		_shader->EndShader();
	}

}