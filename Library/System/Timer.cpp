#include "DXUT.h"
#include "Timer.h"

namespace KMT 
{

	// タイマーを進める関数
	const bool CTimer::TimeCount()
	{
		if( count < time ){
			++count ;
			return false ;
		}
		else{
			ResetCount() ;
			return true ;
		}
	}


	// タイマーを進める関数その2
	const bool CTimer::TimeCountZero()
	{
		if( 0 == count ){
			++count ;
			return true ;
		}
		else if( count > 0 && count < time ){
			++count ;
			return false ;
		}
		else{
			ResetCount() ;
			return false ;
		}
	}

}