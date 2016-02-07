#include "DXUT.h"
#include "Camera.h"

#include "../../Math/Math.h"

namespace KMT {

	Camera::Camera() : _size(1)
	{ }

	Camera::~Camera() { }

	CameraSP Camera::Create()
	{
		return CameraSP(new Camera());
	}

	void Camera::SetSizeIPO(const float &_Size){ _size = Lerp(_size, _Size, 0.1f); }

	void Camera::UpdateView()
	{
		// ビュー行列, プロジェクション行列を DirectX に設定
		D3DXMatrixLookAtLH(&_matrices[VIEW], (D3DXVECTOR3*)&_view.Eye, (D3DXVECTOR3*)&_view.LookAt, (D3DXVECTOR3*)&_view.Up);
		D3DXMatrixPerspectiveFovLH(&_matrices[PROJECTION], _persective.Angle, _persective.Aspect, _persective.Near, _persective.Far);
	}

	void Camera::Calculate()
	{
		// カメラ座標履歴を取得
		_view.previousEye = _view.Eye;
		// カメラ方向
		_direction = (_view.LookAt - _view.Eye).Normalize();
	}

}