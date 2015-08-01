#pragma once
// 
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <list>
using namespace std;

namespace KMT {

	class CFrameRateCounter
	{
	public :
		// コンストラクタ
		CFrameRateCounter(unsigned int smp = 10);
		// デストラクタ
		virtual ~CFrameRateCounter();

		// FPS値を取得
		double getFrameRate_();
		// サンプル数を変更
		void setSampleNumber_(unsigned int smp);

	private :
		// 使用する計測関数の判定フラグ
		int CounterFlag_;
		// クロックカウント数
		LARGE_INTEGER Counter_;
		// 1秒当たりクロックカウント数(周波数)
		double Frequency_;
		// 以前のクロックカウント数
		LONGLONG PreviousCount_;
		// 以前の時刻（ミリ秒）
		DWORD TGTPreviousCount_;
		// 時間リスト
		list<double> diffTimeList_;
		// 移動平均計算時のデータ数
		UINT Number_;
		// 共通部分の合計値
		double summationTimes_;

	protected :
		// 現在の時刻を取得
		double getCurrentDiffTime_();
		// フレームレートを更新
		double updateFrameRate_(double _Diff);

	};

}