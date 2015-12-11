#pragma once 

#include "../Shader.h"

namespace KMT {
	
	class ShaderLambert : public Shader
	{
	public :
		// デストラクタ
		~ShaderLambert() { }
		// シェーダー生成
		static ShaderSP Create();
		// モデルに対するエフェクト設定
		void ApplyEffect(const CMatrix& rotation, const Vector4& cameraPosition);
	
	private :
		// コンストラクタ
		ShaderLambert();

	};

}