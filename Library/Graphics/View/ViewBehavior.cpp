#include "DXUT.h"
#include "ViewBehavior.h"

#include "../../Extension.h"

namespace KMT {

	tag_Perspective::tag_Perspective() : Angle(D3DX_PI / 4.0f), Aspect((float)SCREEN_WIDTH / SCREEN_HEIGHT), Near(1), Far(10000) { }

	ViewBehavior::ViewBehavior()
	{
		ZeroMemory(_viewFrustum, sizeof(PlaneVolume) * FRUSTUM_MAX);
		ZeroMemory(_matrices, sizeof(Matrix) * ViewOrProjection);
	}

	ViewBehavior::~ViewBehavior() { }

	void ViewBehavior::CalculateViewFrustum()
	{
		// スクリーン座標上の視錐台の各頂点座標
		Vector4 nearTL(-1, 1, 0, 1);
		Vector4 nearTR(1, 1, 0, 1);
		Vector4 nearBL(-1, -1, 0, 1);
		Vector4 nearBR(1, -1, 0, 1);
		Vector4 farTL(-1, 1, 1, 1);
		Vector4 farTR(1, 1, 1, 1);
		Vector4 farBL(-1, -1, 1, 1);
		Vector4 farBR(1, -1, 1, 1);

		// ビュープロジェクション行列の逆行列を求める
		Matrix MVP = _matrices[VIEW] * _matrices[PROJECTION];
		Matrix invMVP;
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

		_viewFrustum[TOP] = PlaneVolume(nearTL, nearTR, farTL);
		_viewFrustum[BOTTOM] = PlaneVolume(nearBL, farBL, nearBR);
		_viewFrustum[LEFT] = PlaneVolume(nearBL, nearTL, farTL);
		_viewFrustum[RIGHT] = PlaneVolume(nearBR, farBR, nearTR);
		_viewFrustum[FRONT] = PlaneVolume(nearBL, nearBR, nearTL);
		_viewFrustum[BACK] = PlaneVolume(farTL, farTR, farBL);
	}

	const bool ViewBehavior::CullsFrustum(const SphereVolume& sphere) const
	{
		for(int i = 0; i < FRUSTUM_MAX; i++){
			// 対象と平面の距離を算出
			float dist = Dot(sphere.Position, _viewFrustum[i].Normal) - _viewFrustum[i].Distance;
			if(dist < -sphere.Radius)
				return true;
		}
		return false;
	}

}