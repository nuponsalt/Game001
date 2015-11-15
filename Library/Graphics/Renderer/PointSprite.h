#pragma once

#include "DXUT.h"

#include "GraphicBehavior.h"

namespace KMT {

	// �|�C���g�X�v���C�g���_�f�[�^
	typedef struct tag_POINTSPRITE
	{
		// ���_���W
		Vector3 _position;
		// �|�C���g�X�v���C�g�T�C�Y
		float _size;
		// ���_�J���[
		DWORD _color;

		// ���_�t�H�[�}�b�g��`
		static const WORD FVF = (D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);

	}POINTSPRITE, *pPOINTSPRITE;

	// �G�~�b�^�[
	class Emitter
	{
	public :
		// ���W
		Vector3 _position;
		// ���o�����x�N�g��
		std::vector<Vector3> _directions;
		// ���x
		std::vector<float> _speeds;
		// �����x
		float _acceleration;

		// �p�[�e�B�N���̎���
		int _span;

		// �`��t���O
		bool _renders;

		// �R���X�g���N�^
		Emitter() : _span(0), _renders(false) { }
		// �f�X�g���N�^
		~Emitter() { } 

	};

	class PointSprite;
	typedef std::shared_ptr<PointSprite> PointSpriteSP;

	// �|�C���g�X�v���C�g(�p�[�e�B�N��)
	class PointSprite : public GraphicBehavior
	{
	public :
		// �f�X�g���N�^
		virtual ~PointSprite();

		// �|�C���g�X�v���C�g����
		static PointSpriteSP Create(const std::string& path, const WORD pointNumber, const float size = 1);
		// �`��
		void Render(const CCamera* camera);

	private :
		// ���_�o�b�t�@
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
		// �e�N�X�`��
		CTextureSP _texture;
		// �G�~�b�^�[�z��(std::vector)
		Emitter* _emitterData;

		// ���_1�̑傫��
		float _pointSize;
		// ���_��
		WORD _pointNumber;

		// �R���X�g���N�^
		// path : �\��t����e�N�X�`���p�X
		// pointNumber : �|�C���g��
		// size : �|�C���g�T�C�Y
		PointSprite(const std::string& path, const WORD pointNumber, const float size = 1);

	};

}