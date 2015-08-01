#pragma once

#include <memory>

namespace KMT
{

	class CTimer ;
	typedef std::shared_ptr< CTimer > CTimerSP ;

	class CTimer
	{
	private :
		int count ;	// カウント
		int time ;	// trueを返す時間

	public :

		// デフォルトコンストラクタ
		CTimer() : count(0), time(0)
		{}

		// コンストラクタ
		// arg1... trueを返すまでの時間
		CTimer(const int _time) : count( 0 ), time( _time )
		{}

		// デストラクタ
		~CTimer(){}

		// タイマーを進める関数
		// ret.... [ false : セットした時間にはまだ達していない ] [ true : セットした時間に達した ]
		// tips... セットした時間になるとtrueを返し、カウントをリセットする
		const bool TimeCount() ;

		// タイマーを進める関数その2
		// ret.... [ false : カウントが0以外 ] [ true : カウントが0 ]
		// tips... TimeCount関数とは違い、カウントが0になったときにtrueを返す。他はTimeCount関数と同じ
		const bool TimeCountZero() ;

		// 時間をセットする関数
		// arg1... セットしたい時間
		void setTime( const int _time ){ time = _time ; count = 0 ; }

		// カウントをリセットする関数
		void ResetCount(){ count = 0 ; }

		// 現在セットしてある時間を取得する関数
		// ret.... [ 戻り値 : 現在セットしてある時間 ]
		const int getTime() const { return time ; }

		// 現在のカウントの取得関数
		// ret.... [ 戻り値 : 現在のカウント ]
		const int getCount() const { return count ; }
	} ;

}