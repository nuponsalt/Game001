#pragma once

#include <memory>
#include "GraphicBehavior.h"

namespace KMT {

	// ラインの頂点を定義
	typedef struct LineVertex {
		float x, y, z;
		DWORD color;

		// コンストラクタ
		LineVertex() { }
		LineVertex(CVector3 _v, DWORD _color) : x(_v.x), y(_v.y), z(_v.z), color(_color) { }

	}*pLineVertex;

	class CLine;
	typedef std::shared_ptr<CLine> CLineSP;
	typedef std::weak_ptr<CLine> CLineWP;

	// ライン描画
	class CLine : public CGraphicBehavior
	{
	public :
		// デストラクタ
		~CLine() { }
		// 生成
		static CLineSP Create(const CVector3& _posA, const CVector3& _posB, DWORD _color);

		// 描画
		void Render(const CCamera* _camera);

	private :
		// ラインの頂点
		LineVertex Vertices[2];

		// コンストラクタ
		CLine() { }

	protected :
	};

}