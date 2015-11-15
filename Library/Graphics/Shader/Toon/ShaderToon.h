#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class CTexture;
	typedef std::shared_ptr<CTexture> CTextureSP;

	class ShaderToon : public Shader
	{
	public :
		// �f�X�g���N�^
		~ShaderToon() { }
	
		// �V�F�[�_�[����
		static ShaderSP Create();
		// ���f���ɑ΂���G�t�F�N�g�ݒ�
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);
	
	private :
		// �g�D�[���e�N�X�`���[
		CTextureSP _toonTexture;

		// �R���X�g���N�^
		ShaderToon();
	
	protected :
	};

}
