#pragma once

#pragma warning(disable : 4482)

#include "../../Math/Transformation.h"
#include "../Material.h"
#include "../Mesh.h"
#include "../Texture.h"

#include <vector>
#include <memory>

namespace KMT {

	//* 前方宣言 *//
	class CCamera;
	class CShader;
	typedef std::shared_ptr<CShader> CShaderSP;

	class CGraphicBehavior;
	typedef std::shared_ptr<CGraphicBehavior> CGraphicBehaviorSP;
	typedef std::weak_ptr<CGraphicBehavior> CGraphicBehaviorWP;

	class CGraphicBehavior : public CTransformation, public CMaterial
	{
	public :
		// ソート用の値
		float CameraDistance;
		// 描画フラグ
		bool isRender;
		// ビルボードフラグ
		bool isBillBoard;

		// コンストラクタ
		CGraphicBehavior() : CameraDistance(0), isRender(true), isBillBoard(false), d3dCull(D3DCULL_CCW){ }
		// デストラクタ
		virtual ~CGraphicBehavior() { }

		//* 取得 *//
		// メッシュ
		inline CMeshSP getpMesh() { return Mesh; }
		// テクスチャベクター配列
		inline std::vector<CTextureSP> getptextures() { return Textures; }
		// テクスチャベクター配列(直接インデックス指定)
		inline CTextureSP getptexturesArray(size_t nAry) { return Textures[nAry]; }
		// 拡散反射色(ディフューズ色)
		inline CVector4 getDiffuseColors(size_t nAry) { return DiffuseColors[nAry]; }

		//* 設定 *//
		// シェーダー
		inline void setShader(CShaderSP sdr) { Shader = sdr; }
		// カリングモード
		inline void setd3dCull(const _D3DCULL _d3dCull) { d3dCull = _d3dCull; }
		// テクスチャ
		inline void setTexture(const CTextureSP& _ptex) { Textures.push_back(_ptex); }
		
		//* std::listのsort使用関数 *//
		static inline bool comp(const CGraphicBehaviorWP riLeft, const CGraphicBehaviorWP riRight)
		{
			return (riRight.lock()->CameraDistance < riLeft.lock()->CameraDistance);
		}
		static inline bool compBack(const CGraphicBehaviorWP riLeft, const CGraphicBehaviorWP riRight)
		{
			return (riRight.lock()->CameraDistance < riLeft.lock()->CameraDistance);
		}
		static inline bool comp2D(const CGraphicBehaviorWP riLeft, const CGraphicBehaviorWP riRight)
		{
			return (riRight.lock()->Position.z > riLeft.lock()->Position.z);
		}
		// 描画
		virtual void Render(const CCamera* camera) = 0;

	private :
	protected :
		// ファイルパス
		std::string Path;
		// メッシュ
		CMeshSP Mesh;
		// シェーダー
		CShaderSP Shader;
		// 描画時のカリングモード保存変数
		_D3DCULL d3dCull;
		
		// テクスチャー配列(std::vector)
		std::vector<CTextureSP> Textures;
		// 頂点カラー配列(std::vector)
		std::vector<CVector4> DiffuseColors;

	};

}