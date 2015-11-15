#pragma once

#include "../Shader.h"

namespace KMT {

	class ShaderHalfLambert : public Shader
	{
	public :
		// �f�X�g���N�^
		~ShaderHalfLambert() {}
		// �V�F�[�_�[����
		static ShaderSP Create();
		// ���f���ɑ΂���G�t�F�N�g�ݒ�
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);

	private :
		// �R���X�g���N�^
		ShaderHalfLambert();

	};

}