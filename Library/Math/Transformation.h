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

namespace KMT {

	class Vector3;
	class CMatrix;
	class CQuaternion;

	class Transformation;
	typedef std::shared_ptr<Transformation> CTransformationSP;
	typedef std::weak_ptr<Transformation> CTransformationWP;

	// 回転タイプ
	enum ROTATE_TYPE
	{
			XYZ,
			MATRIX,
			QUATERNION
	};

	class Transformation
	{
	public :
		//* 座標 *//
		Vector3 Position;
		//* 回転 *//
		// ベクトル
		Vector3 vRotation;
		// 行列
		CMatrix mRotationX, mRotationY, mRotationZ, mRotationWorld;
		// クォータニオン
		CQuaternion qRotation;
		//* 拡縮 *//
		Vector3 Scale ;

		// 現在の回転タイプ
		ROTATE_TYPE CurrentRotateType;

		// コンストラクタ
		Transformation();
		// デストラクタ
		virtual ~Transformation() { }
	
};

}