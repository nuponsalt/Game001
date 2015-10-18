//*********************************************
// �X�v���C�g�`��N���X
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#pragma warning ( disable:4238 )

#include "GraphicBehavior.h"
#include "../Animation.h"
#include <memory>

namespace KMT {

	class Sprite ;
	typedef std::shared_ptr<Sprite> SpriteSP ;
	typedef std::weak_ptr<Sprite> SpriteWP ;

	class Sprite : public GraphicBehavior, public CAnimation
	{
	public :
		// �摜�̂ǂ͈̔͂�`�悷�邩
		RECT _rect;
		
		// �f�X�g���N�^
		~Sprite();

		// �e�N�X�`���̐ݒ�
		void SetTexture(const CTextureSP &texture);
		//* ���� *//
		static SpriteSP Create();
		// �ꖇ�e�N�X�`��
		// path : �e�N�X�`���[�̃t�@�C���p�X
		static SpriteSP CreateFromFile(const std::string &path);
		// �A�j���[�V�����e�N�X�`��
		// path : �e�N�X�`���[�̃t�@�C���p�X
		// width : ������
		// height : �c����
		static SpriteSP CreateFromFile(const std::string& path, const int &width, const int &height);
		//* �`�� *//
		void Render(const CCamera* camera = NULL);
		
	private :
		// DirectX3D�X�v���C�g
		LPD3DXSPRITE _sprite;
		// ���]�p�񎟌��x�N�g��
		Vector2 _turnVector;
		
		// �R���X�g���N�^
		Sprite();

	};

}