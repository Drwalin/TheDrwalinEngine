
#ifndef EVENT_RESPONSER_H
#define EVENT_RESPONSER_H

#include <map>
#include <string>

class EventResponser
{
private:
	
	class BasicWindow * basicWindow;
	
	
protected:
	
	class BasicWindow * GetBasicWindow();
	
public:
	
	void SetBasicWindow( class BasicWindow * basicWindow );
	
	virtual void MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw ) = 0;
	virtual void KeyPressedEvent( int keyCode ) = 0;
	virtual void KeyReleasedEvent( int keyCode ) = 0;
	virtual void KeyHoldedEvent( int keyCode ) = 0;
	virtual void StringToEnterEvent( std::string str ) = 0;
	
	EventResponser();
	~EventResponser();
};

#endif

