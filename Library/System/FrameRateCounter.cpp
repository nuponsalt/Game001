#include "DXUT.h"
#include "FrameRateCounter.h"

const int FPSCOUNTER_QUERYPER_COUNTER = 1;
const int FPSCOUNTER_TIMEGETTIME = 2;

namespace KMT {

	FrameRateCounter::FrameRateCounter(unsigned int sample)
	{
		// サンプル数の設定
		SetSampleNumber(sample);
		// 計測する時計の選択
		if(QueryPerformanceCounter(&_counter) != 0)
		{
			// QueryPerformanceCounter関数を使うフラグ
			_counterFlag = FPSCOUNTER_QUERYPER_COUNTER;
			// 生成時の時刻（クロック数）を取得
			_previousCount = _counter.QuadPart;
			LARGE_INTEGER frequency;
			// 1秒当たりクロック数を取得
			QueryPerformanceFrequency(&frequency);
			_frequency = (double)frequency.QuadPart;
		}
		else
		{
			// timeGetTime関数を使うフラグ
			_counterFlag = FPSCOUNTER_TIMEGETTIME;
			// 精度を上げる
			timeBeginPeriod(1);
			// 生成時の時刻（ミリ秒）を取得
			_tgtPreviousCount = timeGetTime();
		}
		// 計測
		GetFrameRate();
	}

	FrameRateCounter::~FrameRateCounter()
	{
		if(_counterFlag == FPSCOUNTER_TIMEGETTIME)
			// タイマーの精度を戻す
			timeEndPeriod(1);
	}

	double FrameRateCounter::GetFrameRate()
	{
		double diff = GetCurrentDiffTime();
		// 計算できないので返す
		if (diff == 0)	return 0;

		return UpdateFrameRate(diff);
	}

	double FrameRateCounter::GetCurrentDiffTime()
	{
		// 差分時間を計測
		if(_counterFlag == FPSCOUNTER_QUERYPER_COUNTER)
		{
			// QueryPerformanceCounter関数による計測
			QueryPerformanceCounter(&_counter); // 現在の時刻を取得し、
			LONGLONG diff = _counter.QuadPart - _previousCount; // 差分カウント数を算出する。
			//double dDef = (double)diff; // 倍精度浮動小数点に変換
			_previousCount = _counter.QuadPart; // 現在の時刻を保持
			return (double)diff * 1000 / _frequency; // 差分時間をミリ秒単位で返す
		}
		// timeGetTime関数による計測
		DWORD CurrentTime = timeGetTime();
		DWORD CurrentDiff = CurrentTime - _tgtPreviousCount; // 差分カウント数を算出する
		_tgtPreviousCount = CurrentTime; // 現在の時刻を保持
		return CurrentDiff;
	}

	double FrameRateCounter::UpdateFrameRate(double diff)
	{
		// 最も古いデータを消去
		_diffTimeList.pop_front();
		// 新しいデータを追加
		_diffTimeList.push_back(diff);
		// フレームレート算出
		double fps = 0.0;
		double averagingDiff = (_summationTimes + diff) / _number;
		if (averagingDiff != 0)
			fps = 1000.0 / averagingDiff;
		// 共通加算部分の更新
		_summationTimes += diff - *_diffTimeList.begin();
		// 結果が戻る
		return fps;
	}

	void FrameRateCounter::SetSampleNumber(unsigned int sample)
	{
		// 平均を計算する個数
		_number = sample;
		// リスト初期化
		_diffTimeList.resize(_number, 0.0);
		_summationTimes = 0;
	}

}