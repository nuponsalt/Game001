#pragma once

#include <string>
#include <memory>
#include "../Graphics/Renderer/Sprite.h"

namespace KMT {

	class CCounter ;
	typedef std::shared_ptr<CCounter> CCounterSP ;

	class CCounter
	{
	public :
		// 寄せタイプ
		enum ALIGN_TYPE{
			// 左寄せ
			LEFT_ALI,	
			// 右寄せ
			RIGHT_ALI	
		};

		// コンストラクタ
		CCounter();
		// デストラクタ
		~CCounter(){ Image.reset(); }
		// 生成
		static CCounterSP CreateCounter() ;
		static CCounterSP CreateCounter(const std::string &_path);
		/* 取得 */
		// 基点座標の取得関数
		inline const CVector3 getPosition() const { return Position; }
		
		inline CSpriteSP getImage() const { return Image; }
		/* 設定 */
		// 座標
		inline void setPosition(const CVector3& _position){ Position = _position; }
		inline void setPosition(const float &_x, const float &_y, const float &_z) { Position = CVector3(_x, _y, _z); }
		// 描画感覚
		inline void setInterval(const float _interval){ Interval = _interval; }
		// 描画関数
		void Render(const ALIGN_TYPE &_type, const int &_num);
		

	private :
		// 横幅に対する分割数
		static const int DIV_X_NUM = 10;
		// 横幅に対する分割数
		static const int DIV_Y_NUM = 1;
		// 表示できる桁数
		static const int MAX_NUM = 10;

		// 数字画像
		CSpriteSP Image;
		// 描画する際の基点座標
		CVector3 Position;
		// 描画間隔
		float Interval;

		// 数字画像のロード関数
		// _path : 使用する数字画像のファイル名
		void LoadFromFile(const std::string& _path);
		
	};

}