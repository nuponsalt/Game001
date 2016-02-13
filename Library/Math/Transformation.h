//*********************************************************************
// 座標・回転・拡縮の管理クラス(リンクライブラリとしての機能実装予定)
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once
#include <memory>

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace KMT
{
	class Vector3;
	class Matrix;
	class Quaternion;

	class Transformation;
	typedef std::shared_ptr<Transformation> CTransformationSP;
	typedef std::weak_ptr<Transformation> CTransformationWP;

	// 回転タイプ
	enum RotationType
	{
		XYZ,
		MATRIX,
		QUATERNION
	};

	class Transformation
	{
	public:
		//* 座標 *//
		Vector3 _position;
		//* 回転 *//
		// ベクトル
		Vector3 _eulerAngles;

		// 行列
		enum RotationMatrixType
		{
			// X軸回転
			X,
			// Y軸回転
			Y,
			// Z軸回転
			Z,
			// 要素数
			ElementAll
		};
		Matrix _rotationMatrices[RotationMatrixType::ElementAll];

		// ワールド回転行列
		Matrix _worldRotationMatrix;

		// クォータニオン
		Quaternion _rotation;

		//* 拡縮 *//
		Vector3 _scale;

		// 現在の回転タイプ
		RotationType CurrentRotateType;

		// コンストラクタ
		Transformation();
		// デストラクタ
		virtual ~Transformation() { }

		// 各行列計算
		void Evaluate()
		{
			auto XAxis = _rotationMatrices[RotationMatrixType::X];
			auto YAsix = _rotationMatrices[RotationMatrixType::Y];
			auto ZAxis = _rotationMatrices[RotationMatrixType::Z];

			switch (CurrentRotateType)
			{
			case RotationType::QUATERNION:
				D3DXMatrixRotationQuaternion(&_worldRotationMatrix, &_rotation);
				break;
			case RotationType::MATRIX:
				_worldRotationMatrix = XAxis * YAsix * ZAxis;
				break;
			case RotationType::XYZ:
				D3DXMatrixRotationYawPitchRoll(&_worldRotationMatrix, _eulerAngles.y, _eulerAngles.x, _eulerAngles.z);
				break;
			}
		}
	};

}