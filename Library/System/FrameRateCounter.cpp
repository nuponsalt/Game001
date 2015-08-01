#include "DXUT.h"
#include "FrameRateCounter.h"

const int FPSCOUNTER_QUERYPER_COUNTER = 1;
const int FPSCOUNTER_TIMEGETTIME = 2;

namespace KMT {

	CFrameRateCounter::CFrameRateCounter(unsigned int smp)
	{
		// サンプル数の設定
		setSampleNumber_(smp);
		// 計測する時計の選択
		if(QueryPerformanceCounter(&Counter_) != 0)
		{
			// QueryPerformanceCounter関数を使うフラグ
			CounterFlag_ = FPSCOUNTER_QUERYPER_COUNTER;
			// 生成時の時刻（クロック数）を取得
			PreviousCount_ = Counter_.QuadPart;
			LARGE_INTEGER Freq;
			// 1秒当たりクロック数を取得
			QueryPerformanceFrequency(&Freq);
			Frequency_ = (double)Freq.QuadPart;
		}
		else
		{
			// timeGetTime関数を使うフラグ
			CounterFlag_ = FPSCOUNTER_TIMEGETTIME;
			// 精度を上げる
			timeBeginPeriod(1);
			// 生成時の時刻（ミリ秒）を取得
			TGTPreviousCount_ = timeGetTime();
		}
		// 計測
		getFrameRate_();
	}

	CFrameRateCounter::~CFrameRateCounter()
	{
		if(CounterFlag_ == FPSCOUNTER_TIMEGETTIME)
			// タイマーの精度を戻す
			timeEndPeriod(1);
	}

	double CFrameRateCounter::getFrameRate_()
	{
		double Diff = getCurrentDiffTime_();
		// 計算できないので返す
		if (Diff == 0)	return 0;

		return updateFrameRate_(Diff);
	}

	double CFrameRateCounter::getCurrentDiffTime_()
	{
		// 差分時間を計測
		if(CounterFlag_ == FPSCOUNTER_QUERYPER_COUNTER)
		{
			// QueryPerformanceCounter関数による計測
			QueryPerformanceCounter(&Counter_); // 現在の時刻を取得し、
			LONGLONG LongDef = Counter_.QuadPart - PreviousCount_; // 差分カウント数を算出する。
			double dDef = (double)LongDef; // 倍精度浮動小数点に変換
			PreviousCount_ = Counter_.QuadPart; // 現在の時刻を保持
			return dDef*1000 / Frequency_; // 差分時間をミリ秒単位で返す
		}
		// timeGetTime関数による計測
		DWORD CurrentTime = timeGetTime();
		DWORD CurrentDiff = CurrentTime - TGTPreviousCount_; // 差分カウント数を算出する
		TGTPreviousCount_ = CurrentTime; // 現在の時刻を保持
		return CurrentDiff;
	}

	double CFrameRateCounter::updateFrameRate_(double _Diff)
	{
		// 最も古いデータを消去
		diffTimeList_.pop_front();
		// 新しいデータを追加
		diffTimeList_.push_back(_Diff);
		// フレームレート算出
		double FPS = 0.0;
		double AveDiff = (summationTimes_ + _Diff) / Number_;
		if (AveDiff != 0)
			FPS = 1000.0 / AveDiff;
		// 共通加算部分の更新
		summationTimes_ += _Diff - *diffTimeList_.begin();
		// 結果が戻る
		return FPS;
	}

	void CFrameRateCounter::setSampleNumber_(unsigned int smp)
	{
		// 平均を計算する個数
		Number_ = smp;
		// リスト初期化
		diffTimeList_.resize(Number_, 0.0);
		summationTimes_ = 0;
	}

}