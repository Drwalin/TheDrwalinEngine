
#ifndef EVENT_H
#define EVENT_H

#include <EventResponser.h>

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>

#include <string>

#include <ctime>

#include <SmartPtr.h>

class Event : public EventResponser
{
private:
	
	class Window * window;
	class Engine * engine;
	
public:
	
	void SetEngine( class Engine * engine );
	void Init();
	virtual void MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw ) override;
	virtual void KeyPressedEvent( int keyCode ) override;
	virtual void KeyReleasedEvent( int keyCode ) override;
	virtual void KeyHoldedEvent( int keyCode ) override;
	virtual void StringToEnterEvent( std::string str ) override;
	
	Event();
	~Event();
};

#endif

