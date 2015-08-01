#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class CTexture;
	typedef std::shared_ptr<CTexture> CTextureSP;

	class CShaderToon : public CShader
	{
	public :
		// デストラクタ
		~CShaderToon() { }
	
		// シェーダー生成
		static CShaderSP CreateShader();
		// モデルに対するエフェクト設定
		void applyEffect(const CMatrix& _rotmtx, const CVector4& _campos);
	
	private :
		// トゥーンテクスチャー
		CTextureSP ToonTexture;

		// コンストラクタ
		CShaderToon();
	
	protected :
	};

}