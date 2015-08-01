//*********************************************************************
// Collision Detect Infomation
// Last Update : 2014.07.19
// Yuta Komatsu
//*********************************************************************
#pragma once

#include <memory>
#include <vector>

#include "BoundingVolume.h"
#include "../Graphics/View/Camera.h"

namespace KMT {

	// 衝突判定補正の方向
	enum COLLIDE_DIRECTION
	{
		COL_ERROR = -0x01,
		COL_NONE = 0x00,
		COL_COLLIDE = 0x01,
		COL_LEFT = 0x02,
		COL_RIGHT = 0x04,
		COL_FRONT = 0x08,
		COL_BACK = 0x10,
		COL_TOP = 0x20,
		COL_BOTTOM = 0x40
	};

	// Z軸無限平面交差判定(補正)
	// posY : 無限平面の高さ
	// posA : オブジェクトの高さ
	// radius : オブジェクトの半径
	bool CorrectInfinitePlane(const float& posY, float& posA, const float& radius);

	// 反射ベクトルの計算
	// pOut : 正規化反射ベクトル
	// Front : 進行方向ベクトル
	// Normal : 衝突点での法線ベクトル
	CVector3* calculateReflectVector(CVector3* pOut, const CVector3& Front, const CVector3& Normal);

	// 壁と球の反射ベクトルを計算
	// pOut : 結果を受け取る, Axis : 反射軸, V : 算出元, Ref : 反発係数
	void getReflectVelocity(CVector3* pOut, CVector3& Axis, CVector3& V, float Ref);

	// 壁との反射後の位置を算出
	void getReflectedPosition(float ResTime, Circle& circle, CVector3& RefV);

	// 取得 : 線分の衝突判定
	// a1 : A線分始端頂点座標, a2 : A線分終端頂点座標
	// b1 : B線分始端頂点座標, b2 : B線分終端頂点座標
	// pOutIP : 交点を格納するCVector3型のポインタ
	bool getCollideSegments(CVector3* pOut, const CVector3& a1, const CVector3& a2, const CVector3& b1, const CVector3& b2);

	// 取得 : 線分と平面の衝突判定
	// 
	bool getCollideSegmentPlane(CVector3* pOut, CVector3 A, CVector3 B, Plane PL);

	// 球が平面の裏側にあるか判定する
	// 
	bool getisBackSpherePlane(const SphereVolume &sphere, const PlaneVolume &plane);

	// 球を基準にした視錐台カリングを判定する
	bool getisCulingFrustumSphere(const CCamera* camera, const SphereVolume& sphere);

	// 取得 : 球同士の衝突判定
	// posA : 円Aの座標, posB : 円Bの座標
	// rA : 円Aの半径, rB : 円Bの半径
	bool getCollideSpheres(CVector3& posA, CVector3& posB, float rA, float rB);

	// パーティクル衝突判定・時刻・位置算出関数
	// rA : パーティクルAの半径, rB : パーティクルBの半径
	// prePosA : パーティクルAの前の位置, posA : パーティクルAの次の到達位置
	// prePosB : パーティクルBの前の位置, posB : パーティクルBの次の到達位置
	// poutTime : 衝突時間を格納するFLOAT型へのポインタ
	// poutCollidedA : パーティクルAの衝突位置を格納するCVector3型へのポインタ, poutColldediB : パーティクルAの衝突位置を格納するCVector3型へのポインタ
	bool calculateParticleCollision(float rA, float rB, 
											CVector3* pPrePosA, CVector3* pPosA,
											CVector3* pPrePosB, CVector3* pPosB,
											float* pOutTime,
											CVector3* pOutCollidedA, CVector3* pOutCollidedB);

	// パーティクル衝突後速度位置算出関数
	// pColPosA : 衝突中のパーティクルAの中心位置, pVelocityA : 衝突の瞬間のパーティクルAの速度
	// pColPosB : 衝突中のパーティクルBの中心位置, pVelocityB : 衝突の瞬間のパーティクルBの速度
	// massA : パーティクルAの質量, massB : パーティクルBの質量
	// resA : パーティクルAの反発率(restitution), resB : パーティクルBの反発率(restitution)
	// time : 反射後の移動可能時間
	// pOutPosA : パーティクルAの反射後位置, pOutVelocityA : パーティクルAの反射後速度ベクトル
	// pOutPosB : パーティクルBの反射後位置, pOutVelocityB : パーティクルBの反射後速度ベクトル
	bool calculateParticlePositionAfterCollision(CVector3* pColPosA, CVector3* pVelocityA,
															CVector3* pColPosB, CVector3* pVelocityB,
															float massA, float massB,
															float resA, float resB,
															float time,
															CVector3* pOutPosA, CVector3* pOutVelocityA,
															CVector3* pOutPosB, CVector3* pOutVelocityB);

}