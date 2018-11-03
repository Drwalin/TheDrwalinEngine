
#ifndef EVENT_RECEIVER_IRRLICHT_H
#define EVENT_RECEIVER_IRRLICHT_H

#include <irrlicht\irrlicht.h>

#include <atomic>
#include <mutex>
#include <vector>
#include <set>

#include <EventResponser.h>
#include <BasicWindow.h>

class EventReceiverIrrlicht : public irr::IEventReceiver
{
private:
	
	std::vector < irr::SEvent > eventQueue;
	std::mutex queueMutex;
	
	EventResponser * eventResponser;
	BasicWindow * basicWindow;
	
	int mouseX, mouseY, mouseW;
	
	std::set < unsigned > keyHolded;
	std::set < unsigned > keyPressed;
	
	void GenerateOneEvent( const irr::SEvent& event );
	
public:
	
	virtual bool OnEvent( const irr::SEvent& event ) override;
	
	void GenerateEvents();
	
	EventReceiverIrrlicht( EventResponser * event, BasicWindow * window );
	~EventReceiverIrrlicht();
};

#endif
