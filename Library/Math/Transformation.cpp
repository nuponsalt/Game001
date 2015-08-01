//*********************************************************************
// 座標・回転・拡縮の管理クラス(リンクライブラリとしての機能実装予定)
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#include "DXUT.h"
#include "Transformation.h"

namespace KMT {

	const float DEFAULT_Y_POSITION = -20.0f;

	CTransformation::CTransformation() : Position(0.0f, DEFAULT_Y_POSITION, 0.0f), vRotation(), qRotation(), Scale(1, 1, 1), CurrentRotateType(ROTATE_TYPE::QUATERNION) { }

}