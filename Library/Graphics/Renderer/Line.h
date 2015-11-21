#pragma once

#include <memory>
#include "GraphicBehavior.h"

namespace KMT {

	// ラインの頂点を定義
	typedef struct LineVertex {
		float _x, _y, _z;
		DWORD _color;

		// コンストラクタ
		LineVertex() { }
		LineVertex(Vector3 v, DWORD color) : _x(v.x), _y(v.y), _z(v.z), _color(color) { }

	}*pLineVertex;

	class Line;
	typedef std::shared_ptr<Line> LineSP;
	typedef std::weak_ptr<Line> LineWP;

	// ライン描画
	class Line : public GraphicBehavior
	{
	public :
		// デストラクタ
		~Line() { }
		// 生成
		static LineSP Create(const Vector3& positionA, const Vector3& positionB, DWORD color);

		// 描画
		void Render(const CCamera* camera);

	private :
		// ラインの頂点
		LineVertex _vertices[2];

		// コンストラクタ
		Line() { }

	protected :
	};

}