
#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

#include <allegro5/allegro.h>

#include <vector>

class TimeCounter
{
private:
	
	struct TimePair
	{
		float begin;
		float end;
	};
	
	std::vector < TimePair > array;
	
	float timeSpan;
	
public:
	
	void SetTimeSpan( float time );
	
	float GetSmoothTime() const;
	
	void SubscribeStart();
	void SubscribeEnd();
	
	TimeCounter();
	~TimeCounter();
};

#endif

