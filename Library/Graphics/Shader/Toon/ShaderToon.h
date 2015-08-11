#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class CTexture;
	typedef std::shared_ptr<CTexture> CTextureSP;

	class ShaderToon : public Shader
	{
	public :
		// デストラクタ
		~ShaderToon() { }
	
		// シェーダー生成
		static ShaderSP CreateShader();
		// モデルに対するエフェクト設定
		void ApplyEffect(const CMatrix& _rotmtx, const CVector4& _campos);
	
	private :
		// トゥーンテクスチャー
		CTextureSP ToonTexture;

		// コンストラクタ
		ShaderToon();
	
	protected :
	};

}
