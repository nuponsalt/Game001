#pragma once

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

#include "../../Phisycs/BoundingVolume.h"

namespace KMT {

	typedef struct tag_View
	{
		// 座標
		CVector3 Eye;
		// 前フレームの座標
		CVector3 previousEye;
		// 注視点
		CVector3 LookAt;
		// 前フレームの注視点
		CVector3 previousLookAt;
		// アッパーベクトル
		CVector3 Up;
		// 前フレームのアッパーベクトル
		CVector3 previousUp;

		// コンストラクタ
		tag_View() : Up(0, 1, 0) { }

	}ViewInfo, *pViewInfo;

	typedef struct tag_Perspective
	{
		// 画角
		float Angle;
		// アスペクト比
		float Aspect;
		// 平面の最近距離
		float Near;
		// 平面の最遠距離
		float Far;

		// コンストラクタ
		tag_Perspective();

	}PerspectiveInfo, *pPerspectiveInfo;

	// ビュー行列・プロジェクション行列を持つオブジェクトの基本情報クラス
	class CViewBehavior
	{
	public :
		// 行列の種類
		enum MatrixState{
			VIEW,
			PROJECTION
		};

		// コンストラクタ
		CViewBehavior();
		// デストラクタ
		virtual ~CViewBehavior();

		// 更新
		virtual void updateView() = 0;

		// 視錐台の算出
		void calculateViewFructum();
		// 球オブジェクトによる視錐台カリング判定
		const bool isCullingFrustum(const SphereVolume& sphere) const;

		// 設定
		inline void setEye(const CVector3 &_Pt) { View.Eye = _Pt; }
		inline void setEye(const float &_fx, const float &_fy, const float &_fz) { View.Eye = CVector3(_fx, _fy, _fz); }
		inline void setLookAt(const CVector3 &_Pt) { View.LookAt = _Pt; }
		inline void setLookAt(const float &_fx, const float &_fy, const float &_fz) { View.LookAt = CVector3(_fx, _fy, _fz); }
		inline void setUp(const CVector3 &_Pt) { View.Up = _Pt; }
		inline void setUp(const float &_fx, const float &_fy, const float &_fz) { View.Up = CVector3(_fx, _fy, _fz); }
		inline void setAspect(const float &_aspect) { Persective.Aspect = _aspect; }

		// 取得 
		inline CVector3 getEye() const { return View.Eye; }
		inline CVector3 getLookAt() const { return View.LookAt; }
		inline float getAngle() const { return Persective.Angle; }
		/* 行列 */
		inline CMatrix getMatrix(const MatrixState& state) const { return Matrix[state]; }

	protected :
		// 視錐台平面列挙型
		enum VIEW_FRUSTUM{
			TOP,
			BOTTOM,
			LEFT,
			RIGHT,
			FRONT,
			BACK,
			// 視錐台の平面の数
			FRUSTUM_MAX
		};
		// 視錐台の6平面
		PlaneVolume viewFrustum[FRUSTUM_MAX];

		// ビュー行列[0], プロジェクション行列[1]
		static const int VIEW_OR_PROJECTION = 2;
		CMatrix Matrix[VIEW_OR_PROJECTION];
		// ビュー情報
		ViewInfo View;
		// プロジェクション
		PerspectiveInfo Persective;

	};

}