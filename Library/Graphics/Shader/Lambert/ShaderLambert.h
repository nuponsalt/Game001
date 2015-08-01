#pragma once 

#include "../Shader.h"

namespace KMT {
	
	class CShaderLambert : public CShader
	{
	public :
		// デストラクタ
		~CShaderLambert() { }
		// シェーダー生成
		static CShaderSP CreateShader();
		// モデルに対するエフェクト設定
		void applyEffect(const CMatrix& _rotmtx, const CVector4& _campos);
	
	private :
		// コンストラクタ
		CShaderLambert();

	};

}