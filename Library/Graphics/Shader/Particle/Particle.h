#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class CShaderParticle;
	typedef std::shared_ptr<CShaderParticle> CShaderParticleSP;
	typedef std::weak_ptr<CShaderParticle> CShaderPartcleWP;

	class CShaderParticle : public Shader
	{
	public :
		// デストラクタ
		~CShaderParticle() { }
		// シェーダー生成
		static ShaderSP CreateShader();
		// モデルに対するエフェクト適用
		void ApplyEffect(const CMatrix& _rotmtx, const CVector4& _campos);

	private :
		// コンストラクタ
		CShaderParticle();

	};
}