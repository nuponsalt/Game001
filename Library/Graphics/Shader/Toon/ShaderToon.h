#pragma once

#include "../Shader.h"
#include <memory>

namespace KMT {

	class Texture;
	typedef std::shared_ptr<Texture> TextureSP;

	class ShaderToon : public Shader
	{
	public :
		// デストラクタ
		~ShaderToon() { }
	
		// シェーダー生成
		static ShaderSP Create();
		// モデルに対するエフェクト設定
		void ApplyEffect(const CMatrix& rotation, const CVector4& cameraPosition);
	
	private :
		// トゥーンテクスチャー
		TextureSP _toonTexture;

		// コンストラクタ
		ShaderToon();
	
	protected :
	};

}
