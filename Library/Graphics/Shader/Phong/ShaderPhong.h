#pragma once

#include "../Shader.h"

namespace KMT {

	class ShaderPhong : public Shader
	{
	public :
		// デストラクタ
		~ShaderPhong() { }

		// 生成
		static ShaderSP Create();
		// モデルに対するエフェクト適用
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);

	private :
		// コンストラクタ
		ShaderPhong();

	protected :
	};

}