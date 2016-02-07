#pragma once

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"

#include "../../Phisycs/BoundingVolume.h"

namespace KMT {

	typedef struct tag_View
	{
		// 座標
		Vector3 Eye;
		// 前フレームの座標
		Vector3 previousEye;
		// 注視点
		Vector3 LookAt;
		// 前フレームの注視点
		Vector3 previousLookAt;
		// アッパーベクトル
		Vector3 Up;
		// 前フレームのアッパーベクトル
		Vector3 previousUp;

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
	class ViewBehavior
	{
	public :
		// 行列の種類
		enum MatrixState{
			VIEW,
			PROJECTION
		};

		// コンストラクタ
		ViewBehavior();
		// デストラクタ
		virtual ~ViewBehavior();

		// 更新
		virtual void UpdateView() = 0;

		// 視錐台の算出
		void CalculateViewFrustum();
		// 球オブジェクトによる視錐台カリング判定
		const bool CullsFrustum(const SphereVolume& sphere) const;

		// 設定
		inline void SetEye(const Vector3 &eye) { _view.Eye = eye; }
		inline void SetEye(const float &x, const float &y, const float &z) { _view.Eye = Vector3(x, y, z); }
		inline void SetLookAt(const Vector3 &lookAt) { _view.LookAt = lookAt; }
		inline void SetLookAt(const float &x, const float &y, const float &z) { _view.LookAt = Vector3(x, y, z); }
		inline void SetUp(const Vector3 &upDirection) { _view.Up = upDirection; }
		inline void SetUp(const float &x, const float &y, const float &z) { _view.Up = Vector3(x, y, z); }
		inline void SetAspect(const float &aspect) { _persective.Aspect = aspect; }

		// 取得 
		inline Vector3 GetEye() const { return _view.Eye; }
		inline Vector3 GetLookAt() const { return _view.LookAt; }
		inline float GetAngle() const { return _persective.Angle; }
		/* 行列 */
		inline Matrix GetMatrix(const MatrixState& state) const { return _matrices[state]; }

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
		PlaneVolume _viewFrustum[FRUSTUM_MAX];

		// ビュー行列[0], プロジェクション行列[1]
		static const int ViewOrProjection = 2;

		Matrix _matrices[ViewOrProjection];
		// ビュー情報
		ViewInfo _view;
		// プロジェクション
		PerspectiveInfo _persective;

	};

}