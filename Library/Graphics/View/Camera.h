//*********************************************
// カメラの基本情報管理クラス
// Last Update : 2013.11.29
// Yuta Komatsu
//*********************************************
#pragma once

#include "ViewBehavior.h"
#include "../../System/Command.h"

#include <memory>

namespace KMT { 

	class Camera;
	typedef std::shared_ptr<Camera> CameraSP;
	typedef std::weak_ptr<Camera> CameraWP;

	class Camera : public ViewBehavior
	{
	public :
		// デストラクタ
		virtual ~Camera();

		// 生成
		static CameraSP Create();
		
		// 設定
		inline void SetSize(const float &size) { _size = size; }
		void SetSizeIPO(const float &_Size);
		/* コマンド */
		inline void AttachCommand(const Command& command) { _command = command; }
		
		// 取得
		inline Vector3 GetDirection() const { return _direction; } //0ベクトルになるような部分での引用はバグの温床になるからNG
		inline float GetSize() const { return _size; }

		// 行列更新
		void UpdateView();
		// 各カメラ座標算出
		virtual void Calculate();

	protected :
		// カメラの向き
		Vector3 _direction;		
		// サイズ
		float _size;
		// コマンド
		Command _command;

		// コンストラクタ
		Camera();

	} ;

}