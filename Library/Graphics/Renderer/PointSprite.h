#pragma once

#include "DXUT.h"

#include "GraphicBehavior.h"

namespace KMT {

	// ポイントスプライト頂点データ
	typedef struct tag_POINTSPRITE
	{
		// 頂点座標
		CVector3 Position;
		// ポイントスプライトサイズ
		float PSize;
		// 頂点カラー
		DWORD Color;

		// 頂点フォーマット定義
		static const WORD FVF = (D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE);

	}POINTSPRITE, *pPOINTSPRITE;

	// エミッター
	class CEmitter
	{
	public :
		// 座標
		CVector3 Position;
		// 放出方向ベクトル
		std::vector<CVector3> Directions;
		// 速度
		std::vector<float> Speeds;
		// 加速度
		float acceleration;

		// パーティクルの寿命
		int Span;

		// 描画フラグ
		bool isRender;

		// コンストラクタ
		CEmitter() : Span(0), isRender(false) { }
		// デストラクタ
		~CEmitter() { } 

	};

	class CPointSprite;
	typedef std::shared_ptr<CPointSprite> CPointSpriteSP;

	// ポイントスプライト(パーティクル)
	class CPointSprite : public CGraphicBehavior
	{
	public :
		// デストラクタ
		virtual ~CPointSprite();

		// ポイントスプライト生成
		static CPointSpriteSP CreatePointSprite(const std::string& path, const WORD numPoint, const float size = 1);
		// 描画
		void Render(const CCamera* camera);

	private :
		// 頂点バッファ
		LPDIRECT3DVERTEXBUFFER9 pd3dVertexBuffer;
		// テクスチャ
		CTextureSP Texture;
		// エミッター配列(std::vector)
		CEmitter* EmitterData;

		// 頂点1つの大きさ
		float pointSize;
		// 頂点数
		WORD pointNumber;

		// コンストラクタ
		// path : 貼り付けるテクスチャパス
		// numPoint : ポイント数
		// size : ポイントサイズ
		CPointSprite(const std::string& path, const WORD numPoint, const float size = 1);

	};

}