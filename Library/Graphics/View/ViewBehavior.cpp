#include "DXUT.h"
#include "ViewBehavior.h"

#include "../../Extension.h"

namespace KMT {

	tag_Perspective::tag_Perspective() : Angle(D3DX_PI / 4.0f), Aspect((float)SCREEN_WIDTH / SCREEN_HEIGHT), Near(1), Far(10000) { }

	CViewBehavior::CViewBehavior()
	{
		ZeroMemory(viewFrustum, sizeof(PlaneVolume) * FRUSTUM_MAX);
		ZeroMemory(Matrix, sizeof(CMatrix) * VIEW_OR_PROJECTION);
	}

	CViewBehavior::~CViewBehavior() { }

	void CViewBehavior::calculateViewFructum()
	{
		// スクリーン座標上の視錐台の各頂点座標
		CVector4 nearTL(-1, 1, 0, 1);
		CVector4 nearTR(1, 1, 0, 1);
		CVector4 nearBL(-1, -1, 0, 1);
		CVector4 nearBR(1, -1, 0, 1);
		CVector4 farTL(-1, 1, 1, 1);
		CVector4 farTR(1, 1, 1, 1);
		CVector4 farBL(-1, -1, 1, 1);
		CVector4 farBR(1, -1, 1, 1);

		// ビュープロジェクション行列の逆行列を求める
		CMatrix MVP = Matrix[VIEW] * Matrix[PROJECTION];
		CMatrix invMVP;
		D3DXMatrixInverse(&invMVP, NULL, &MVP);

		// ワールド座標上の視錐台の各頂点を求める
		nearTL.Transform(invMVP);
		nearTR.Transform(invMVP);
		nearBL.Transform(invMVP);
		nearBR.Transform(invMVP);
		farTL.Transform(invMVP);
		farTR.Transform(invMVP);
		farBL.Transform(invMVP);
		farBR.Transform(invMVP);

		// wで除算
		nearTL /= nearTL.w;
		nearTR /= nearTR.w;
		nearBL /= nearBL.w;
		nearBR /= nearBR.w;
		farTL /= farTL.w;
		farTR /= farTR.w;
		farBL /= farBL.w;
		farBR /= farBR.w;

		viewFrustum[TOP] = PlaneVolume(nearTL, nearTR, farTL);
		viewFrustum[BOTTOM] = PlaneVolume(nearBL, farBL, nearBR);
		viewFrustum[LEFT] = PlaneVolume(nearBL, nearTL, farTL);
		viewFrustum[RIGHT] = PlaneVolume(nearBR, farBR, nearTR);
		viewFrustum[FRONT] = PlaneVolume(nearBL, nearBR, nearTL);
		viewFrustum[BACK] = PlaneVolume(farTL, farTR, farBL);
	}

	const bool CViewBehavior::isCullingFrustum(const SphereVolume& sphere) const
	{
		for(int i = 0; i < FRUSTUM_MAX; i++){
			// 対象と平面の距離を算出
			float dist = Dot(sphere.Position, viewFrustum[i].Normal) - viewFrustum[i].Distance;
			if(dist < -sphere.Radius)
				return true;
		}
		return false;
	}

}