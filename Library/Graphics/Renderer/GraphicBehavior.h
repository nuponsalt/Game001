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
	class Camera;
	class Shader;
	typedef std::shared_ptr<Shader> ShaderSP;

	class GraphicBehavior;
	typedef std::shared_ptr<GraphicBehavior> GraphicBehaviorSP;
	typedef std::weak_ptr<GraphicBehavior> GraphicBehaviorWP;

	class GraphicBehavior : public Transformation, public Material
	{
	public :
		// ソート用の値
		float _cameraDistance;
		// 描画フラグ
		bool _renders;
		// ビルボードフラグ
		bool _isBillBoard;

		// コンストラクタ
		GraphicBehavior() : _cameraDistance(0), _renders(true), _isBillBoard(false), _cullingState(D3DCULL_CCW){ }
		// デストラクタ
		virtual ~GraphicBehavior() { }

		//* 取得 *//
		// メッシュ
		inline MeshSP GetMesh() { return _mesh; }
		// テクスチャベクター配列
		inline std::vector<TextureSP> GetTextures() { return _textures; }
		// テクスチャベクター配列(直接インデックス指定)
		inline TextureSP GetTexturesElement(size_t index) { return _textures[index]; }
		// 拡散反射色(ディフューズ色)
		inline Vector4 GetDiffuseColors(size_t index) { return _diffuseColors[index]; }

		//* 設定 *//
		// シェーダー
		inline void SetShader(ShaderSP shader) { _shader = shader; }
		// カリングモード
		inline void SetCullingState(const _D3DCULL cullingState) { _cullingState = cullingState; }
		// テクスチャ
		inline void SetTexture(const TextureSP& texture) { _textures.push_back(texture); }
		
		//* std::listのsort使用関数 *
		static inline bool Compare(const GraphicBehaviorWP riLeft, const GraphicBehaviorWP riRight)
		{
			return (riRight.lock()->_cameraDistance < riLeft.lock()->_cameraDistance);
		}
		static inline bool CompareBack(const GraphicBehaviorWP riLeft, const GraphicBehaviorWP riRight)
		{
			return (riRight.lock()->_cameraDistance < riLeft.lock()->_cameraDistance);
		}
		static inline bool Compare2D(const GraphicBehaviorWP riLeft, const GraphicBehaviorWP riRight)
		{
			return (riRight.lock()->_position.z > riLeft.lock()->_position.z);
		}

		// 描画
		virtual void Render(const Camera* camera) = 0;

	private :
	protected :
		// ファイルパス
		std::string _path;
		// メッシュ
		MeshSP _mesh;
		// シェーダー
		ShaderSP _shader;
		// 描画時のカリングモード保存変数
		_D3DCULL _cullingState;
		
		// テクスチャー配列(std::vector)
		std::vector<TextureSP> _textures;
		// 頂点カラー配列(std::vector)
		std::vector<Vector4> _diffuseColors;

	};

}