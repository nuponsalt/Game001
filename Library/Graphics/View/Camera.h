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

	class CCamera;
	typedef std::shared_ptr<CCamera> CCameraSP;
	typedef std::weak_ptr<CCamera> CCameraWP;

	class CCamera : public CViewBehavior
	{
	public :
		// デストラクタ
		virtual ~CCamera();

		// 生成
		static CCameraSP Create();
		
		// 設定
		inline void setSize(const float &_Size) { Size = _Size; }
		void setSizeIPO(const float &_Size);
		/* コマンド */
		inline void setCommand(const Command& _command) { Command = _command; }
		
		// 取得
		inline Vector3 getDirection() const { return Direction; } //0ベクトルになるような部分での引用はバグの温床になるからNG
		inline float getSize() const { return Size; }

		// 行列更新
		void updateView();
		// 各カメラ座標算出
		virtual void Calculate();

	protected :
		// カメラの向き
		Vector3 Direction;		
		// サイズ
		float Size;
		// コマンド
		Command Command;

		// コンストラクタ
		CCamera();

	} ;

}