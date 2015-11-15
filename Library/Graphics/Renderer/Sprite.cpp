#include "DXUT.h"
#include "Sprite.h"
#include "../GraphicsManager.h"

namespace KMT {

	Sprite::Sprite() : _turnVector(1, 1)
	{
		// �X�v���C�g�̐���
		D3DXCreateSprite(GraphicsManager::_device, &_sprite) ;
	}

	Sprite::~Sprite()
	{
		Texture.reset() ;
		SAFE_RELEASE( _sprite ) ;
	}

	void Sprite::SetTexture(const CTextureSP &texture) 
	{
		Texture = texture ;
	}

	SpriteSP Sprite::CreateFromFile(const std::string &path)
	{
		SpriteSP object(new Sprite()) ;
		// �e�N�X�`����ݒ�
		object->LoadTextureAndAnimation(path) ;

		return object ;
	}

	SpriteSP Sprite::CreateFromFile(const std::string &path, const int &width, const int &height) 
	{
		SpriteSP object(new Sprite()) ;
		// �e�N�X�`���̐ݒ�
		object->LoadTextureAndAnimation(path, width, height) ;

		return object ;
	}

	void Sprite::Render(const CCamera* camera)
	{
		// �`�悷�邩
		if(!_renders)
			return;

		// ���[���h�s��Ɋւ�����
		D3DXMATRIX WldMtx, PosMtx, SclMtx, RotMtx; 
		// �g�����X���[�V����( ���s�ړ� ) �s��̍쐬
		D3DXMatrixTranslation(&PosMtx, Position.x, Position.y, Position.z);
		// �X�P�[��( �g�k ) �s��̍쐬
		D3DXMatrixScaling(&SclMtx, _turnVector.x * Scale.x, _turnVector.y * Scale.y, 1.0f);
		// ���[�e�[�V����( ��] ) �s��̍쐬 
		// �� ����� 2D�Ȃ̂� Z��]�̂�
		D3DXMatrixRotationZ(&RotMtx, D3DXToRadian(vRotation.z));
		//�y !�d�v! �z���[���h�s��̎Z�o 
		// DirectX �ł� ���[���h�s��̎Z�o��
		// �X�P�[�� * ���[�e�[�V���� * �g�����X���[�V���� �̏��ōs��
		WldMtx = SclMtx * RotMtx * PosMtx;
		// �X�v���C�g�Ƀ��[���h�s���ݒ�
		_sprite->SetTransform(&WldMtx);

		HRESULT hr;
		// Render the scene
		if(SUCCEEDED( DXUTGetD3D9Device()->BeginScene()))
		{		
			// �X�v���C�g�̕`�揀��
			_sprite->Begin(D3DXSPRITE_ALPHABLEND); // �������A�s�����̂Ƃ��͂O��n��
			//----------------------------------------------------------------------------------
			//
			// ���Z�u�����h�̐ݒ�
			//
			if(_addsBlend)
			{
				DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				DXUTGetD3D9Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}/*else{
				GraphicsManager::_device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ) ;
			}*/

			// �X�v���C�g�̕`�施��
			_sprite->Draw(
				// �`��Ɏg�p����e�N�X�`��
				Texture->getpd3dTexture(),
				// �摜�̕`��͈�
				&Rects[Number],	
				// �T�[�t�F�C�X�̒��S
				(D3DXVECTOR3*)&CenterPosition,
				// �`����W(�s��Ő��䂷��̂�0, 0, 0��OK)
				&D3DXVECTOR3(0,0,0),
				// �F
				D3DCOLOR_ARGB((int)(_colorRGBA.w * 255), (int)(_colorRGBA.x * 255), (int)(_colorRGBA.y * 255), (int)(_colorRGBA.z * 255)));

			// ���Z�؂�ւ�
			_sprite->End();
			// �`��I��
			V(GraphicsManager::_device->EndScene());
		}
	}

}