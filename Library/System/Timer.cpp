#include "DXUT.h"
#include "Timer.h"

namespace KMT 
{

	// タイマーを進める関数
	const bool Timer::Count()
	{
		if( _count < _time )
		{
			++_count ;
			return false ;
		}
		else
		{
			ResetCount() ;
			return true ;
		}
	}


	// タイマーを進める関数その2
	const bool Timer::CountZero()
	{
		if( 0 == _count )
		{
			++_count ;
			return true ;
		}
		else if( _count > 0 && _count < _time )
		{
			++_count ;
			return false ;
		}
		else
		{
			ResetCount() ;
			return false ;
		}
	}

}