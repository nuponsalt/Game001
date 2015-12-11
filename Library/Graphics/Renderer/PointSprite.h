#pragma once

#include "DXUT.h"

#include "GraphicBehavior.h"

namespace KMT {

	// ポイントスプライト頂点データ
	typedef struct tag_POINTSPRITE
	{
		// 頂点座標
		Vector3 _position;
		// ポイントスプライトサイズ
		float _size;
		// 頂点カラー
		DWORD _color;

		// 頂点フォーマット定義
		static const WORD FVF = (D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);

	}POINTSPRITE, *pPOINTSPRITE;

	// エミッター
	class Emitter
	{
	public :
		// 座標
		Vector3 _position;
		// 放出方向ベクトル
		std::vector<Vector3> _directions;
		// 速度
		std::vector<float> _speeds;
		// 加速度
		float _acceleration;

		// パーティクルの寿命
		int _span;

		// 描画フラグ
		bool _renders;

		// コンストラクタ
		Emitter() : _span(0), _renders(false) { }
		// デストラクタ
		~Emitter() { } 

	};

	class PointSprite;
	typedef std::shared_ptr<PointSprite> PointSpriteSP;

	// ポイントスプライト(パーティクル)
	class PointSprite : public GraphicBehavior
	{
	public :
		// デストラクタ
		virtual ~PointSprite();

		// ポイントスプライト生成
		static PointSpriteSP Create(const std::string& path, const WORD pointNumber, const float size = 1);
		// 描画
		void Render(const CCamera* camera);

	private :
		// 頂点バッファ
		LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;
		// テクスチャ
		TextureSP _texture;
		// エミッター配列(std::vector)
		Emitter* _emitterData;

		// 頂点1つの大きさ
		float _pointSize;
		// 頂点数
		WORD _pointNumber;

		// コンストラクタ
		// path : 貼り付けるテクスチャパス
		// pointNumber : ポイント数
		// size : ポイントサイズ
		PointSprite(const std::string& path, const WORD pointNumber, const float size = 1);

	};

}