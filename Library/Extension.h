//*********************************************
// 名前空間オブジェクト管理/ヘルパー関数管理
// Last Update : 2013.12.9
// Yuta Komatsu
//*********************************************
#pragma once

#include <memory>
#include "Math/Vector.h"

namespace KMT {

	const int SCREEN_WIDTH = 960, SCREEN_HEIGHT = 540;

	//* システム *//
	// マルチバイト文字型変換
	HRESULT DXconvAnsiToWide(WCHAR* wstrDest, const CHAR* strSrc, int ct);
	// 範囲指定乱数生成
	// min : 範囲最小値, max : 範囲最大値
	int GetRandom(const int &min, const int &max);

	//* 数学 *//
	// 体積計算(球)
	// _rad : 半径
	float calculateSphereVolume(float _rad);
	// 体積計算(直方体)
	// _width : 幅, _height : 高さ, _depth : 深さ 
	float calculateCuboidVolume(float _width, float _height, float _depth);
	
	// クォータニオンによるベクトルの回転
	// v : 計算の基になるベクトル, qtn : 計算の基になるクォータニオン
	D3DXVECTOR3 QuaternionTransformCoord(D3DXVECTOR3 v, D3DXQUATERNION qtn);
	
	// 2Dベクトルの外積計算 
	float Vector2Cross(CVector2& v1, CVector2& v2);
	
	// 線分上の最近点を得る
	// p : 基になる座標ベクトル, a : 線分の始点, b : 線分の終点
	CVector3 NearOnLine(const CVector3 &p, const CVector3 &a, const CVector3 &b);
	
	// 3頂点で構成される平面の法線を算出
	// v1 : 基準点, 
	CVector3 NormalOnPlane(const CVector3 vA, const CVector3& vB, const CVector3& vC);
}