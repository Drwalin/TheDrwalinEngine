
#ifndef TIME_COUNTER_CPP
#define TIME_COUNTER_CPP

#include <TimeCounter.h>

#include <Debug.h>

void TimeCounter::SetTimeSpan( float time )
{
	timeSpan = time;
}

float TimeCounter::GetSmoothTime() const
{
	float ret = 0.000001f;
	for( int i = 0; i < array.size(); ++i )
		ret += array[i].end - array[i].begin;
	if( array.size() )
	{
		ret -= 0.000001f;
		ret /= float(array.size());
	}
	return ret;
}

void TimeCounter::SubscribeStart()
{
	array.resize( array.size() + 1 );
	array.back().begin = al_get_time();
	array.back().end = array.back().begin;
}

void TimeCounter::SubscribeEnd()
{
	array.back().end = al_get_time();
	
	if( array.size() )
	{
		float maxTime = array.back().end - timeSpan;
		
		for( int i = array.size()-1; i >= 0; --i )
		{
			if( array[i].end < maxTime )
			{
				array.erase( array.begin(), array.begin()+i+1 );
				break;
			}
		}
	}
}

TimeCounter::TimeCounter()
{
	timeSpan = 0.5f;
}

TimeCounter::~TimeCounter()
{
	array.clear();
	timeSpan = 0.0f;
}

#endif

