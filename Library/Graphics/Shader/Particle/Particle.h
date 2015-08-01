#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class CShaderParticle;
	typedef std::shared_ptr<CShaderParticle> CShaderParticleSP;
	typedef std::weak_ptr<CShaderParticle> CShaderPartcleWP;

	class CShaderParticle : public CShader
	{
	public :
		// デストラクタ
		~CShaderParticle() { }
		// シェーダー生成
		static CShaderSP CreateShader();
		// モデルに対するエフェクト適用
		void applyEffect(const CMatrix& _rotmtx, const CVector4& _campos);

	private :
		// コンストラクタ
		CShaderParticle();

	};
}