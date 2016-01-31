#pragma once

#include <memory>

namespace KMT
{

	class Timer ;
	typedef std::shared_ptr< Timer > TimerSP ;

	class Timer
	{
	private :
		int _count ;	// カウント
		int _time ;	// trueを返す時間

	public :

		// デフォルトコンストラクタ
		Timer() : _count(0), _time(0)
		{}

		// コンストラクタ
		// arg1... trueを返すまでの時間
		Timer(const int time) : _count( 0 ), _time( time )
		{}

		// デストラクタ
		~Timer(){}

		// タイマーを進める関数
		// ret.... [ false : セットした時間にはまだ達していない ] [ true : セットした時間に達した ]
		// tips... セットした時間になるとtrueを返し、カウントをリセットする
		const bool Count() ;

		// タイマーを進める関数その2
		// ret.... [ false : カウントが0以外 ] [ true : カウントが0 ]
		// tips... Count関数とは違い、カウントが0になったときにtrueを返す。他はCount関数と同じ
		const bool CountZero() ;

		// 時間をセットする関数
		// arg1... セットしたい時間
		void SetTime( const int time ){ _time = time ; _count = 0 ; }

		// カウントをリセットする関数
		void ResetCount(){ _count = 0 ; }

		// 現在セットしてある時間を取得する関数
		// ret.... [ 戻り値 : 現在セットしてある時間 ]
		const int GetTime() const { return _time ; }

		// 現在のカウントの取得関数
		// ret.... [ 戻り値 : 現在のカウント ]
		const int GetCount() const { return _count ; }
	} ;

}