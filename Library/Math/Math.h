//*********************************************
// 演算の拡張関数
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#include <DirectXMath.h>

namespace KMT {

	class Vector3;
	class Quaternion;

	// X軸周りの回転を表すクォータニオンを取得
	Quaternion RotationX(float angle);
	// Y軸周りの回転を表すクォータニオンを取得
	Quaternion RotationY(float angle);
	// Z軸周りの回転を表すクォータニオンを取得
	Quaternion RotationZ(float angle);

	// 線形補間
	// 結果のfloat値を生成
	// f=0のときa、f=1のときbとなる
	inline float Lerp(float a, float b, float f)
	{
		return a * (1 - f) + b * f;
	}

	// 球面線形補間
	// f = 0のときa, f = 1のときb
	// 機能は該当するコンストラクタと同等
	Quaternion SphericalLerp(const Quaternion& a, const Quaternion& b, float f);

	// クォータ二オンの差分1
	// return : 演算結果のクォータニオン
	// tips1 : a -> b への差分
	// tips2 : この関数の戻り値はoutの戻り値と同じである。別の関数の引数としても扱える。
	Quaternion* QuaternionDelta(Quaternion *out, const Quaternion *a, const Quaternion *b);
}