#pragma once 

#pragma warning( disable:4482 )

#include "GraphicBehavior.h"

#include <vector>
#include <memory>
#include <string>
#include <sstream>

namespace KMT {

	class CCamera;

	class CModelRenderer;
	typedef std::shared_ptr<CModelRenderer> CModelRendererSP, CGRendererSP;
	typedef std::weak_ptr<CModelRenderer> CModelRendererWP, CGRendererWP;

	typedef class CModelRenderer : public CGraphicBehavior
	{
	public :
		// コンストラクタ
		CModelRenderer();
		// デストラクタ
		virtual ~CModelRenderer(){ Textures.clear(); }
		
		// 生成
		static CModelRendererSP CreateFromX(const std::string &_path, const CShaderSP &_shader);
		// 描画
		virtual void Render(const CCamera* camera);		

	private :		
		// Xファイル読込
		void LoadFromX(const std::string &_path);

	}CGRenderer;

}