#pragma once

#pragma warning( disable:4018 )
#pragma warning( disable:4244 )

#include <memory>
#include "../GraphicBehavior.h"
#include "../../Animation.h"

namespace KMT
{
	class GraphicalPlane;
	typedef std::shared_ptr<GraphicalPlane> GraphicalPlaneSP, GPlaneSP;

	typedef class GraphicalPlane : public GraphicBehavior, public CAnimation
	{
	public :
		// �T�C�Y
		Vector3 _size;

		// �f�X�g���N�^
		~GraphicalPlane(){}
		// ����
		// width : �����T�C�Y
		// height : �c���T�C�Y
		// texture : �e�N�X�`���[(��łȂ��ƃ_���ȏ��)
		static GraphicalPlaneSP Create(const int &width, const int &height, const CTextureSP &texture = NULL) ;
		// �e�N�X�`���[�̃T�C�Y�ɍ��킹���|���S���̐���
		// path : �e�N�X�`���[�̃p�X
		// divisionX, divisionY : ������
		// sizeX, sizeY : �T�C�Y
		static GraphicalPlaneSP CreateFromTexture(const std::string &path, 
			const int &divisionX = 1, const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0) ;
		static GraphicalPlaneSP CreateFromTexture(const CTextureSP &texture, 
			const int &divisionX = 1, const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0) ;
		// �`��
		void Render(const CCamera* camera);

	private :
		// �e�N�X�`���[�̃T�C�Y
		Vector3 _textureSize;
		// �O�̃A�j���[�V�����ԍ�
		int _previousNumber;
		// �|���̐����J�E���g
		static size_t _createCount;

		// �R���X�g���N�^
		GraphicalPlane();
		// �|���S���̍쐬
		// width : �����T�C�Y
		// height : �c���T�C�Y
		// texture : �e�N�X�`���[
		bool GenerateBoard(const std::string& path, const int &width, const int &height, const CTextureSP &texture = NULL) ;
		// �e�N�X�`���̃T�C�Y�ɍ��킹���|���𐶐�����
		// path : �e�N�X�`���̃p�X
		// divisionX, divisionY : �摜�̕������
		// sizeX, sizeY : �|���̑傫���w��
		void LoadTexture (const std::string &path, const int &divisionX = 1, const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0);
		void LoadTexture (const CTextureSP& texture, const int &divisionX = 1,const int &divisionY = 1, const int &sizeX = 0, const int &sizeY = 0);

	}GPlane;

}