#pragma once

#include "../Shader.h"

namespace KMT {

	class ShaderHalfLambert : public Shader
	{
	public :
		// デストラクタ
		~ShaderHalfLambert() {}
		// シェーダー生成
		static ShaderSP Create();
		// モデルに対するエフェクト設定
		void ApplyEffect(const Matrix& rotation, const Vector4& cameraPosition);

	private :
		// コンストラクタ
		ShaderHalfLambert();

	};

}