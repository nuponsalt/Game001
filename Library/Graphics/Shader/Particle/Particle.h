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
		// デストラクタ
		~ShaderParticle() { }
		// シェーダー生成
		static ShaderSP Create();
		// モデルに対するエフェクト適用
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);

	private :
		// コンストラクタ
		ShaderParticle();

	};
}