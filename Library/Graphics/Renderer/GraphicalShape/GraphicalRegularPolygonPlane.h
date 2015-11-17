#pragma once

#pragma warning(disable:4018)
#pragma warning(disable:4244)

#include <memory>
#include "../GraphicBehavior.h"
#include "../../Animation.h"

namespace KMT {

	enum FaceGenerateType
	{
		// 面ごとに3頂点
		TRI_VTX,
		// 角数+1 = 頂点数
		POLYNUM_VTX
	};

	class GraphicalRegularPolygonPlane;
	typedef std::shared_ptr<GraphicalRegularPolygonPlane> GraphicalRegularPolygonPlaneSP, RegPolyPlaneSP;
	
	class GraphicalRegularPolygonPlane : public GraphicBehavior
	{
	public :
		// サイズ
		CVector3 _size ;

		// デストラクタ
		~GraphicalRegularPolygonPlane(){}
		// 生成
		// radius : 横幅サイズ
		// faceNumber : 縦幅サイズ
		// type : 面生成タイプ
		// texture : テクスチャー
		static RegPolyPlaneSP Create(const int &radius, const int &faceNumber, const FaceGenerateType &type, const TextureSP &texture = NULL);
	
	private :
		CVector3 _textureSize ;

		// コンストラクタ
		GraphicalRegularPolygonPlane();
		// 正多角形板ポリゴンの作成(正三角形以上)
		// radius : 半径
		// faceNumber : 面数(3で正三角形,数が増えるほど円に近くなる)
		// texture : テクスチャー
		bool GenerateRegularPolygonBoard(const int &radius, const int &faceNumber, const FaceGenerateType &type, const TextureSP &texture = NULL);
	};

}