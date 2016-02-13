#include "DXUT.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace KMT {

	Matrix::Matrix(const Vector3& scale, const Quaternion& rotation, const Vector3& position)
	{
		D3DXMatrixTransformation(this, 0, 0, (D3DXVECTOR3*)&scale, 0, (D3DXQUATERNION*)&rotation, (D3DXVECTOR3*)&position);
	}

	// 代入
	Matrix& Matrix::operator= (const Matrix& m)
	{
		memcpy(this, &m, sizeof(Matrix));
		return *this;
	}
}