
#ifndef EVENT_RESPONSER_CPP
#define EVENT_RESPONSER_CPP

#include <EventResponser.h>

BasicWindow * EventResponser::GetBasicWindow()
{
	return basicWindow;
}

void EventResponser::SetBasicWindow( BasicWindow * basicWindow )
{
	this->basicWindow = basicWindow;
}

EventResponser::EventResponser()
{
	basicWindow = NULL;
}

EventResponser::~EventResponser()
{
	basicWindow = NULL;
}

#endif

