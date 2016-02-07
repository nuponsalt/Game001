#pragma once

#include <string>
#include <memory>
#include "../Graphics/Renderer/Sprite.h"

namespace KMT {

	class Counter ;
	typedef std::shared_ptr<Counter> CounterSP ;

	class Counter
	{
	public :
		// 寄せタイプ
		enum AlignState{
			// 左寄せ
			LEFT_ALIGN,	
			// 右寄せ
			RIGHT_ALIGN	
		};

		// コンストラクタ
		Counter();
		// デストラクタ
		~Counter(){ _image.reset(); }
		// 生成
		static CounterSP CreateCounter() ;
		static CounterSP CreateCounter(const std::string &_path);
		/* 取得 */
		// 基点座標の取得関数
		inline const Vector3 GetPosition() const { return _position; }
		
		inline SpriteSP GetImage() const { return _image; }
		/* 設定 */
		// 座標
		inline void SetPosition(const Vector3& position){ _position = position; }
		inline void SetPosition(const float &x, const float &y, const float &z) { _position = Vector3(x, y, z); }
		// 描画感覚
		inline void SetInterval(const float interval){ _interval = interval; }
		// 描画関数
		void Render(const AlignState &type, const int &num);
		

	private :
		// 横幅に対する分割数
		static const int DIV_X_NUM = 10;
		// 横幅に対する分割数
		static const int DIV_Y_NUM = 1;
		// 表示できる桁数
		static const int MAX_NUM = 10;

		// 数字画像
		SpriteSP _image;
		// 描画する際の基点座標
		Vector3 _position;
		// 描画間隔
		float _interval;

		// 数字画像のロード関数
		// _path : 使用する数字画像のファイル名
		void LoadFromFile(const std::string& _path);
		
	};

}