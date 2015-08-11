#pragma once

#include "../Shader.h"

namespace KMT {

	class ShaderPhong : public Shader
	{
	public :
		// デストラクタ
		~ShaderPhong() { }

		// 生成
		static ShaderSP CreateShader();
		// モデルに対するエフェクト適用
		void ApplyEffect(const CMatrix& rotation, const CVector4& cameraPosition);

	private :
		// コンストラクタ
		ShaderPhong();

	protected :
	};

}