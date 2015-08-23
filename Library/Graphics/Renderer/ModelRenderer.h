#pragma once 

#pragma warning( disable:4482 )

#include "GraphicBehavior.h"

#include <vector>
#include <memory>
#include <string>
#include <sstream>

namespace KMT {

	class CCamera;

	class ModelRenderer;
	typedef std::shared_ptr<ModelRenderer> ModelRendererSP, GRendererSP;
	typedef std::weak_ptr<ModelRenderer> ModelRendererWP, GRendererWP;

	typedef class ModelRenderer : public GraphicBehavior
	{
	public :
		// コンストラクタ
		ModelRenderer();
		// デストラクタ
		virtual ~ModelRenderer(){ _textures.clear(); }
		
		// 生成
		static ModelRendererSP CreateFromX(const std::string &path, const ShaderSP &shader);
		// 描画
		virtual void Render(const CCamera* camera);		

	private :		
		// Xファイル読込
		void LoadFromX(const std::string &path);

	}GRenderer;

}