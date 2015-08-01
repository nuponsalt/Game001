#pragma once

#pragma warning(disable:4018)
#pragma warning(disable:4244)

#include <memory>
#include "../GraphicBehavior.h"
#include "../../Animation.h"

namespace KMT {

	enum FACE_GENERATE_TYPE
	{
		// 面ごとに3頂点
		TRI_VTX,
		// 角数+1 = 頂点数
		POLYNUM_VTX
	};

	class CGraphicalRegularPolygonPlane;
	typedef std::shared_ptr<CGraphicalRegularPolygonPlane> CGraphicalRegularPolygonPlaneSP, CRegPolyPlaneSP;
	
	class CGraphicalRegularPolygonPlane : public CGraphicBehavior
	{
	public :
		// サイズ
		CVector3 Size ;

		// デストラクタ
		~CGraphicalRegularPolygonPlane(){}
		// 生成
		// _radius : 横幅サイズ
		// _facenum : 縦幅サイズ
		// _type : 面生成タイプ
		// _texture : テクスチャー
		static CRegPolyPlaneSP Create(const int &_radius, const int &_facenum, const FACE_GENERATE_TYPE &_type, const CTextureSP &_texture = NULL);
	
	private :
		CVector3 TextureSize ;

		// コンストラクタ
		CGraphicalRegularPolygonPlane();
		// 正多角形板ポリゴンの作成(正三角形以上)
		// _radius : 半径
		// _facenum : 面数(3で正三角形,数が増えるほど円に近くなる)
		// _texture : テクスチャー
		bool GenerateRegularPolygonBoard(const int &_radius, const int &_facenum, const FACE_GENERATE_TYPE &_type, const CTextureSP &_texture = NULL);
	};

}