#pragma once

#include "../Shader.h"

namespace KMT {

	class CShaderPhong : public CShader
	{
	public :
		// デストラクタ
		~CShaderPhong() { }

		// 生成
		static CShaderSP CreateShader();
		// モデルに対するエフェクト適用
		void applyEffect(const CMatrix& _rotmtx, const CVector4& _campos);

	private :
		// コンストラクタ
		CShaderPhong();

	protected :
	};

}