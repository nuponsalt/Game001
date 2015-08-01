//*********************************************************************
// Bounding Volume Infomation
// Last Update : 2014.7.19
// Yuta Komatsu
//*********************************************************************
#pragma once
#pragma warning(disable : 4238)
#pragma warning(disable : 4239)

#include <vector>
#include <memory>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace KMT {

	//* typedef宣言 *//
	struct BoundingVolume;
	typedef std::shared_ptr<BoundingVolume> BoundingVolumeSP, BVSP;

	struct Sphere;
	typedef std::shared_ptr<Sphere> SphereSP;

	//* クラス宣言 *//
	// 衝突ボリューム基底構造体
	struct BoundingVolume
	{
		// 番号
		DWORD ID;
		// 判定
		bool isDetect;
		// 速度
		CVector3 Velocity;
		// 加速度
		CVector3 Acceleration;
		// 質量
		float Mass;
		// スケール
		float Scale;
		
		// コンストラクタ
		BoundingVolume() : ID(0), Mass(1.0f), Scale(1.0f), isDetect(true)
		{
			static int cnt;
			ID = cnt++;
		}

		virtual ~BoundingVolume() { }
	};

	// 線分構造体
	struct Segment : public BoundingVolume
	{
		// 始点座標
		CVector3 s;
		// 終点座標
		CVector3 e;

		// コンストラクタ(全ての頂点が減点の線分生成)
		Segment() { }
		// コンストラクタ(指定した頂点座標の線分を生成)
		Segment(const CVector3 &_s, const CVector3 &_e) : s(_s), e(_e) { }

	};

	// 平面構造体
	typedef struct Plane : public BoundingVolume
	{
		// 法線
		CVector3 Normal;
		// 無限平面上の点から減点までの距離
		float Distance;

		// コンストラクタ
		Plane() : Normal(0, 1, 0), Distance(0) { }
		/// 与えられた3点から平面を初期化する
		/// 引数は時計回りの順で与える
		Plane(const CVector3& v1, const CVector3& v2, const CVector3& v3);

	}PlaneVolume, *pPlaneVolume;

	// 球構造体(円としても利用可能)
	typedef struct Sphere : public BoundingVolume
	{
		// 円の座標
		CVector3 Position;
		// 1つ前の座標
		CVector3 PreviousPosition;
		// 半径
		float Radius;

		// コンストラクタ
		Sphere() : Radius(1.0f) { }
	
	}CSVolume, Circle, CircleVolume, *PCircleVolume, SphereVolume, *PSphereVolume;
	
	// 三角形構造体
	typedef struct Triangle : public BoundingVolume
	{
		// 頂点座標
		CVector3 Position[3];
		// 1つ前の頂点座標
		CVector3 PreviousPosition[3];

		// コンストラクタ
		Triangle() { for (int i = 0; i < 3; i++) { Position[i] = PreviousPosition[i] = 0; } }

		// 取得 : 三角形の法線
		CVector3 getNomal() const { return Normalize(Cross(Position[1] - Position[0], Position[2] - Position[0])); }

	}*PTriangle, PolygonVolume, *PPolygonVolume;

	struct AABB : public BoundingVolume
	{
		// 中心座標
		CVector3 Position;
		// 1つ前の中心座標
		CVector3 PreviousPosition;
		// 半径
		CVector3 Radius;

		// コンストラクタ
		AABB() { }

	};
}

