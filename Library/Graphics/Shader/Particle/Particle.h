#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class ShaderParticle;
	typedef std::shared_ptr<ShaderParticle> ShaderParticleSP;
	typedef std::weak_ptr<ShaderParticle> ShaderPartcleWP;

	class ShaderParticle : public Shader
	{
	public :
		// �f�X�g���N�^
		~ShaderParticle() { }
		// �V�F�[�_�[����
		static ShaderSP Create();
		// ���f���ɑ΂���G�t�F�N�g�K�p
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);

	private :
		// �R���X�g���N�^
		ShaderParticle();

	};
}