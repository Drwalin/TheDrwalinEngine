
#ifndef STRING_TO_ENTER_H
#define STRING_TO_ENTER_H

#include <irrlicht\irrlicht.h>

#include <string>
#include <map>

class StringToEnter
{
private:
	
	class BasicWindow * basicWindow;
	
	std::string str;
	int currentPosition;
	std::map < int, float > cooldownToUseKey;
	
	unsigned charactersLimit;
	
	void InsertSign( char sign );
	
public:
	
	void SetCharactersLimit( unsigned value );
	
	void SetBasicWindow( class BasicWindow * basicWindow );
	
	void PressKey( const irr::SEvent::SKeyInput & key );
	
	void Clear();
	std::string GetCurrent();
	
	StringToEnter();
	~StringToEnter();
};

#endif

