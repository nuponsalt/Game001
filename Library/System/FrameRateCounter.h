#pragma once

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <list>
using namespace std;

namespace KMT {

	class FrameRateCounter
	{
	public :
		// コンストラクタ
		FrameRateCounter(unsigned int sample = 10);
		// デストラクタ
		virtual ~FrameRateCounter();

		// FPS値を取得
		double GetFrameRate();
		// サンプル数を変更
		void SetSampleNumber(unsigned int sample);

	private :
		// 使用する計測関数の判定フラグ
		int _counterFlag;
		// クロックカウント数
		LARGE_INTEGER _counter;
		// 1秒当たりクロックカウント数(周波数)
		double _frequency;
		// 以前のクロックカウント数
		LONGLONG _previousCount;
		// 以前の時刻（ミリ秒）
		DWORD _tgtPreviousCount;
		// 時間リスト
		list<double> _diffTimeList;
		// 移動平均計算時のデータ数
		UINT _number;
		// 共通部分の合計値
		double _summationTimes;

	protected :
		// 現在の時刻を取得
		double GetCurrentDiffTime();
		// フレームレートを更新
		double UpdateFrameRate(double diff);

	};

}