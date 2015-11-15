#pragma once

#include "../Shader.h"

namespace KMT {

	class ShaderPhong : public Shader
	{
	public :
		// �f�X�g���N�^
		~ShaderPhong() { }

		// ����
		static ShaderSP Create();
		// ���f���ɑ΂���G�t�F�N�g�K�p
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);

	private :
		// �R���X�g���N�^
		ShaderPhong();

	protected :
	};

}