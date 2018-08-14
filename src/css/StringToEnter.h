
#ifndef STRING_TO_ENTER_H
#define STRING_TO_ENTER_H

#include <string>
#include <map>

#include <Keyboard.h>

enum UpdateStringToEnterActions
{
	ACTION_STRING_TO_ENTER_HOLDED,
	ACTION_STRING_TO_ENTER_PRESSED
};

class StringToEnter
{
private:
	
	class BasicWindow * basicWindow;
	
	std::string str;
	int currentPosition;
	std::map < int, float > cooldownToUseKey;
	
public:
	
	void SetBasicWindow( class BasicWindow * basicWindow );
	
	void InsertSign( char sign );
	void InsertKey( int keyCode, bool capsLock, bool isShift );
	void Update( int keyCode, int action );
	void Clear();
	std::string GetCurrent();
	
	StringToEnter();
	~StringToEnter();
};

#endif

