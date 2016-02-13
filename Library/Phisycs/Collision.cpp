#include "DXUT.h"
#include "Collision.h"
#include "../Extension.h"


namespace KMT {

	bool CorrectInfinitePlane(const float &posY, float &posA,const float &radius)
	{
		// 対象半径で判定
		if(posY > (posA - radius))
		{
			// 補正
			posA = posY + radius ;
			return true ;	// 真
		}
		return false ;	// 偽
	}

	Vector3* calculateReflectVector(Vector3 *out, const Vector3 &front, const Vector3 &normal)
	{
		Vector3 N;
		auto nml = normal;
		N = nml.Normalize();

		auto forward = front;
		out = &(forward - 2.0f * forward.Dot(N) * N).Normalize();//Vector3Dot(&Front, &N) * N).
		return out;
	}

	void getReflectVelocity(Vector3 *pOut, Vector3 &Axis,Vector3 &V, float Ref)
	{
		Normalize(Axis);
		*pOut = V - (1 + Ref) * Dot(Axis, V) * Axis;
	}

	void getReflectedPosition(float ResTime, Circle &circle, Vector3 &RefV)
	{
		// 衝突位置
		// 0.99は壁にめり込まないための補正
		circle.Position = circle.PreviousPosition + circle.Velocity * (1 - ResTime) * 0.99f;
		// 反射ベクトル
		circle.Velocity = RefV;
		// 位置を補正
		circle.Position += circle.Velocity * ResTime;
	}

	bool getCollideSegments(Vector3 *pOut, const Vector3 &a1, const Vector3 &a2, const Vector3 &b1, const Vector3 &b2)
	{
		Vector2 seg1_s = a1;
		Vector2 seg1_e = a2;

		Vector2 seg2_s = b1;
		Vector2 seg2_e = b2;

		Vector2 seg1_v = seg1_e - seg1_s;
		Vector2 seg2_v = seg2_e - seg2_s;

		Vector2 v = seg2_s - seg1_s;

		float C_v1_v2 = Vector2Cross(seg1_v, seg2_v);
		if (C_v1_v2 == 0.0f) {
			// 平行状態
			return false;
		}

		float C_v_v1 = Vector2Cross(v, seg1_v);
		float C_v_v2 = Vector2Cross(v, seg2_v);

		float t1 = C_v_v2 / C_v1_v2;
		float t2 = C_v_v1 / C_v1_v2;

		const float eps = 0.00001f;
		if ( t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1 ) {
			// 交差していない
			return false;
		}

		Vector2 pp = seg1_s + seg1_v * t1 ;
		pOut = (Vector3*)&pp;

		return true;
	}

	//bool getCollideSegmentPlane(CVector3 *pOut, CVector3 A, CVector3 B, Plane PL)
	//{
	//	//平面上の点P
	//	CVector3 P = CVector3(PL.a * PL.d, PL.b * PL.d, PL.c * PL.d);
	//	//PA PBベクトル
	//	CVector3 pA = P - A;
	//	CVector3 pB = P - B;
	//	//PA PBそれぞれを平面法線と内積
	//	double dotPA = (pA.x * PL.a) + (pA.y * PL.b) + (pA.z * PL.c);
	//	double dotPB = (pB.x * PL.a) + (pB.y * PL.b) + (pB.z * PL.c);
	//	//これは線端が平面上にあった時の計算の誤差を吸収しています。調整して使ってください。
	//	if (abs(dotPA) < 0.000001) { dotPA = 0.0; }	
	//	if (abs(dotPB) < 0.000001) { dotPB = 0.0; }
	//	//交差判定
	//	if (dotPA == 0.0 && dotPB == 0.0) 
	//		//両端が平面上にあり、交点を計算できない。
	//		return false;
	//
	//	else if ((dotPA >= 0.0 && dotPB <= 0.0) || (dotPA <= 0.0 && dotPB >= 0.0)) {
	//		//内積の片方がプラスで片方がマイナスなので、交差している
	//	} else 
	//		//交差していない
	//		return false;
	//
	//	//以下、交点を求める 
	//	CVector3 AB = B - A;
	//	//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB
	//	float distance = (float)abs(dotPA) / ((float)abs(dotPA) + (float)abs(dotPB));
	//
	//	pOut = &(A + (AB * distance));
	//
	//	return true;
	//
	//}

	bool getisBackSpherePlane(const SphereVolume& sphere, const PlaneVolume& plane)
	{
		// 平面に対する距離を算出
		float dist = Dot(sphere.Position, plane.Normal) - plane.Distance;

		// 距離が負の半径より小さい場合は裏側にある
		return dist < -sphere.Radius;
	}

	// カメラに対する視錐台平面を生成する
	//void CreateViewFrustumPlane(const CCamera* camera, PlaneVolume* outputs)
	//{
	//	// スクリーン座標上の視錐台の各頂点を算出する
	//	D3DXVECTOR4 NTL(-1,  1, 0, 1); // near top left
	//	D3DXVECTOR4 NTR( 1,  1, 0, 1); // near top right
	//	D3DXVECTOR4 NBL(-1, -1, 0, 1); // near bottom left
	//	D3DXVECTOR4 NBR( 1, -1, 0, 1); // near bottom right
	//	D3DXVECTOR4 FTL(-1,  1, 1, 1); // far top left
	//	D3DXVECTOR4 FTR( 1,  1, 1, 1); // far top right
	//	D3DXVECTOR4 FBL(-1, -1, 1, 1); // far bottom left
	//	D3DXVECTOR4 FBR( 1, -1, 1, 1); // far bottom right

	//	// カメラのビュープロジェクション行列の逆行列を算出する
	//	D3DXMATRIX INVCVP = 
	//		camera->getMatrix(CViewBehavior::VIEW) * camera->getMatrix(CViewBehavior::PROJECTION);
	//	D3DXMatrixInverse(&INVCVP, NULL, &INVCVP);

	//	// カメラ座標系の視錐台の各頂点を算出する
	//	D3DXVec3Transform(&NTL, (D3DXVECTOR3*)&NTL, &INVCVP);
	//	D3DXVec3Transform(&NTR, (D3DXVECTOR3*)&NTR, &INVCVP);
	//	D3DXVec3Transform(&NBL, (D3DXVECTOR3*)&NBL, &INVCVP);
	//	D3DXVec3Transform(&NBR, (D3DXVECTOR3*)&NBR, &INVCVP);
	//	D3DXVec3Transform(&FTL, (D3DXVECTOR3*)&FTL, &INVCVP);
	//	D3DXVec3Transform(&FTR, (D3DXVECTOR3*)&FTR, &INVCVP);
	//	D3DXVec3Transform(&FBL, (D3DXVECTOR3*)&FBL, &INVCVP);
	//	D3DXVec3Transform(&FBR, (D3DXVECTOR3*)&FBR, &INVCVP);

	//	// wで除算する
	//	NTL /= NTL.w;
	//	NTR /= NTR.w;
	//	NBL /= NBL.w;
	//	NBR /= NBR.w;
	//	FTL /= FTL.w;
	//	FTR /= FTR.w;
	//	FBL /= FBL.w;
	//	FBR /= FBR.w;

	//	// スクリーン座標上の頂点から平面を初期化する
	//	outputs[0].Initialize((D3DXVECTOR3)NTR, (D3DXVECTOR3)NTL, (D3DXVECTOR3)NBL); // near
	//	outputs[1].Initialize((D3DXVECTOR3)FBL, (D3DXVECTOR3)FTL, (D3DXVECTOR3)FTR); // far
	//	outputs[2].Initialize((D3DXVECTOR3)NBR, (D3DXVECTOR3)FBR, (D3DXVECTOR3)FTR); // right
	//	outputs[3].Initialize((D3DXVECTOR3)FTL, (D3DXVECTOR3)FBL, (D3DXVECTOR3)NBL); // left
	//	outputs[4].Initialize((D3DXVECTOR3)FTL, (D3DXVECTOR3)NTL, (D3DXVECTOR3)NTR); // top
	//	outputs[5].Initialize((D3DXVECTOR3)NBR, (D3DXVECTOR3)NBL, (D3DXVECTOR3)FBL); // bottom
	//}

	//bool getisCulingFrustumSphere(const CCamera* camera, const SphereVolume& sphere)
	//{
	//	// 平面を生成する
	//	PlaneVolume planes[FRUSTUM_FACE];
	//	CreateFrustumCullingPlane(camera, planes);
	//}

	bool getCollideSpheres(Vector3& posA, Vector3& posB, float rA, float rB) 
	{
		return LengthSq(posA - posB) < ((rA + rB) * (rA + rB));
	}
	
	bool calculateParticleCollision(float rA, float rB, 
									Vector3 *pPrePosA, Vector3 *pPosA,
									Vector3 *pPrePosB, Vector3 *pPosB,
									float *pOutTime,
									Vector3 *pOutCollidedA, Vector3 *pOutCollidedB)
	{
		// 前位置及び到達位置におけるパーティクル間のベクトルを算出
		Vector3 C0 = *pPrePosB - *pPrePosA;
		Vector3 C1 = *pPosB - *pPosA;
		Vector3 D = C1 - C0;
		// 衝突判定用の2次関数係数の算出
		float P = LengthSq(D); if(P == 0) return false; // 同じ方向に移動
		float Q = Dot(C0, D);
		float R = LengthSq(C0);
		// パーティクル距離
		float r = rA + rB;
		// 衝突判定式
		float Judge = (Q * Q) - (P * (R - (r * r)));
		if(Judge < 0){
			// 衝突していない
			return false;
		}
		// 衝突時間の算出
		float t_plus = (-Q + sqrt(Judge)) / P;
		float t_minus = (-Q - sqrt(Judge)) / P;
		// 衝突時間が0未満1より大きい場合、衝突しない
		if( (t_plus < 0 || t_plus > 1) && (t_minus < 0 || t_minus > 1)) return false;
		// 衝突時間の決定（t_minus側が常に最初の衝突）
		*pOutTime = t_minus;
		// 衝突位置の決定
		*pOutCollidedA = *pPrePosA + t_minus * (*pPosA - *pPrePosA);
		*pOutCollidedB = *pPrePosB + t_minus * (*pPosB - *pPrePosB);
		// 衝突報告
		return true;
	}

	bool calculateParticlePositionAfterCollision(Vector3 *pColPosA, Vector3 *pVelocityA,
											Vector3 *pColPosB, Vector3 *pVelocityB,
											float massA, float massB,
											float resA, float resB,
											float time,
											Vector3 *pOutPosA, Vector3 *pOutVelocityA,
											Vector3 *pOutPosB, Vector3 *pOutVelocityB)
	{
		// 質量の合計
		float TotalMass = massA + massB;
		// 反発率
		float RefRate = (1 + resA * resB);
		// 衝突軸ベクトル
		Vector3 C = *pColPosB - *pColPosA;
		Normalize(C);
		Vector3 V = (*pVelocityA - *pVelocityB);
		// 内積算出
		float D = Dot(V, C);
		// 定数ベクトル
		const Vector3 VEC = RefRate * D / TotalMass * C;
		// 衝突後速度ベクトルの算出
		*pOutVelocityA = -massB * VEC + *pVelocityA;
		*pOutVelocityB = massA * VEC + *pVelocityB;
		// 衝突後位置の算出
		*pOutPosA = *pColPosA + time * (*pOutVelocityA);
		*pOutPosB = *pColPosB + time * (*pOutVelocityB);

		return true;
	}

}