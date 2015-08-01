#pragma once

#include "../Shader.h"

namespace KMT {

	class CShaderHalfLambert : public CShader
	{
	public :
		// デストラクタ
		~CShaderHalfLambert() {}
		// シェーダー生成
		static CShaderSP CreateShader();
		// モデルに対するエフェクト設定
		void applyEffect(const CMatrix& _rotmtx, const CVector4& _campos);

	private :
		// コンストラクタ
		CShaderHalfLambert();

	};

}