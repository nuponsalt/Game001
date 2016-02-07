//*********************************************************************
// クォータニオンクラス
// Last Update : 2013.12.11
// Yuta Komatsu
//*********************************************************************
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

namespace KMT
{
	// クォータニオン
	class Quaternion : public D3DXQUATERNION
	{
	public:
		// コンストラクタ
		// 単位クォータニオンを生成
		Quaternion() : D3DXQUATERNION(0, 0, 0, 1) { }
		// 指定した要素で生成
		Quaternion(float x, float y, float z, float w) : D3DXQUATERNION(x, y, z, w) { }
		// 指定した軸と角度の回転を表すクォータニオンを生成
		// 指定角度はラジアン値
		Quaternion(const Vector3& axis, float angle);
		// 指定した軸と角度の回転を表すクォータニオンを生成
		// 上記コンストラクタと機能は同等
		friend Quaternion RotateAroundAxis(const Vector3& axis, float angle)
		{
			return Quaternion(axis, angle);
		}
		// 指定軸の回転を表すクォータニオンを生成
		// 軸の長さが角度になる
		Quaternion(const Vector3& axis);
		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		Quaternion(Vector3 from, Vector3 to);
		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		// 上記コンストラクタと機能は同等
		friend Quaternion RotationBetweenVectors(const Vector3& from, const Vector3& to)
		{
			return Quaternion(from, to);
		}
		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		// 指定した軸を中心に回転するので、カメラなどで天地を反転させたくない場合などに使用
		Quaternion(Vector3 from, Vector3 to, Vector3 axis);
		// 指定ベクトル間の回転を表すクォータニオンを生成
		// ベクトルは正規化しておく必要ない
		// 指定した軸を中心に回転するので、カメラなどで天地を反転させたくない場合などに使用
		// 機能は該当するコンストラクタと同等
		friend Quaternion RotationBetweenVectorsAroundAxis(const Vector3& from, const Vector3& to, const Vector3& axis)
		{
			return Quaternion(from, to, axis);
		}
		// 球面線形補間
		// f = 0のときv1, f = 1のときv2
		Quaternion(const Quaternion& a, const Quaternion& b, float f);
		// D3DXQUATERNIONからの変換
		Quaternion(const D3DXQUATERNION& a);
		// D3DXQUATERNIONへの変換
		operator D3DXQUATERNION() const;
		// 代入
		Quaternion& operator= (const Quaternion& a);
		// 正規化
		// 既存のクォータニオンに代入
		Quaternion& Normalize();
		// 正規化
		// 結果のクォータニオンを生成
		friend Quaternion Normalize(Quaternion a)
		{
			return q.Normalize();
		}
		// 逆クォータニオン
		// 既存のクォータニオンに代入
		Quaternion& Inverse();
		// 逆クォータニオン
		// 結果のクォータニオンを生成
		friend Quaternion Inverse(Quaternion a)
		{
			return q.Inverse();
		}
		// 差分
		// 既存のクォータニオンに代入
		Quaternion& Delta(Quaternion a);
		// 差分
		// 結果のクォータニオンに代入
		friend Quaternion Delta(Quaternion a, Quaternion b)
		{
			return a.Delta(b);
		}
	};
}