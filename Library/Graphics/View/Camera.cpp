#include "DXUT.h"
#include "Camera.h"

#include "../../Math/Math.h"

namespace KMT {

	CCamera::CCamera() : Size(1)
	{ }

	CCamera::~CCamera() { }

	CCameraSP CCamera::Create()
	{
		return CCameraSP(new CCamera());
	}

	void CCamera::setSizeIPO(const float &_Size){ Size = Interpolate(Size, _Size, 0.1f); }

	void CCamera::updateView()
	{
		// ビュー行列, プロジェクション行列を DirectX に設定
		D3DXMatrixLookAtLH(&Matrix[VIEW], (D3DXVECTOR3*)&View.Eye, (D3DXVECTOR3*)&View.LookAt, (D3DXVECTOR3*)&View.Up);
		D3DXMatrixPerspectiveFovLH(&Matrix[PROJECTION], Persective.Angle, Persective.Aspect, Persective.Near, Persective.Far);
	}

	void CCamera::Calculate()
	{
		// カメラ座標履歴を取得
		View.previousEye = View.Eye;
		// カメラ方向
		Direction = (View.LookAt - View.Eye).Normalize();
	}

}