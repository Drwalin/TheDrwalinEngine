
#ifndef STRING_TO_ENTER_CPP
#define STRING_TO_ENTER_CPP

#include <StringToEnter.h>
#include <BasicWindow.h>

void StringToEnter::SetBasicWindow( BasicWindow * basicWindow )
{
	this->basicWindow = basicWindow;
}

std::string StringToEnter::GetCurrent()
{
	return str;
}

void StringToEnter::Clear()
{
	str.clear();
	currentPosition = 0;
}

void StringToEnter::InsertSign( char sign )
{
	str.insert( currentPosition, 1, sign );
	++currentPosition;
}

void StringToEnter::InsertKey( int keyCode, bool capsLock, bool isShift )
{
	if( keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z )
	{
		InsertSign( ( (capsLock^isShift) ? 'A' : 'a' ) + keyCode - ALLEGRO_KEY_A );
	}
	else if( ( keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9 ) || ( keyCode >= ALLEGRO_KEY_PAD_0 && keyCode <= ALLEGRO_KEY_PAD_9 ) )
	{
		if( keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9 )
			keyCode -= ALLEGRO_KEY_0;
		else
			keyCode -=ALLEGRO_KEY_PAD_0;
		
		if( isShift && keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9 )
		{
			switch( keyCode )
			{
			case 0:
				InsertSign( '!' );
				break;
			case 1:
				InsertSign( '@' );
				break;
			case 2:
				InsertSign( '#' );
				break;
			case 3:
				InsertSign( '$' );
				break;
			case 4:
				InsertSign( '%' );
				break;
			case 5:
				InsertSign( '^' );
				break;
			case 6:
				InsertSign( '&' );
				break;
			case 7:
				InsertSign( '*' );
				break;
			case 8:
				InsertSign( '(' );
				break;
			case 9:
				InsertSign( ')' );
				break;
			}
		}
		else
		{
			InsertSign( '0' + keyCode );
		}
	}
	else
	{
		switch( keyCode )
		{
		case ALLEGRO_KEY_TAB:///////////////////////////////////////////////////////////////
			if( isShift )
			{
				if( currentPosition > 0 )
				{
					if( str[currentPosition-1] == '\t' )
					{
						str.erase( str.begin()+currentPosition-1 );
						--currentPosition;
					}
				}
			}
			else
			{
				InsertSign( '\t' );
			}
			break;
		case ALLEGRO_KEY_SPACE:
			InsertSign( ' ' );
			break;
		case ALLEGRO_KEY_TILDE:
			InsertSign( !isShift ? '`' : '~' );
			break;
		case ALLEGRO_KEY_MINUS:
			InsertSign( !isShift ? '-' : '_' );
			break;
		case ALLEGRO_KEY_EQUALS:
			InsertSign( !isShift ? '=' : '+' );
			break;
		case ALLEGRO_KEY_OPENBRACE:
			InsertSign( !isShift ? '[' : '{' );
			break;
		case ALLEGRO_KEY_CLOSEBRACE:
			InsertSign( !isShift ? ']' : '}' );
			break;
		case ALLEGRO_KEY_SEMICOLON:
			InsertSign( !isShift ? ';' : ':' );
			break;
		case ALLEGRO_KEY_QUOTE:
			InsertSign( !isShift ? '\'' : '"' );
			break;
		case ALLEGRO_KEY_BACKSLASH:
			InsertSign( !isShift ? '\\' : '|' );
			break;
		case ALLEGRO_KEY_BACKSLASH2:
			InsertSign( !isShift ? '\\' : '|' );	/////////////////////////
			break;
		case ALLEGRO_KEY_COMMA:
			InsertSign( !isShift ? ',' : '<' );
			break;
		case ALLEGRO_KEY_FULLSTOP:
			InsertSign( !isShift ? '.' : '>' );
			break;
		case ALLEGRO_KEY_SLASH:
			InsertSign( !isShift ? '/' : '?' );
			break;
		case ALLEGRO_KEY_PAD_SLASH:
			InsertSign( '/' );
			break;
		case ALLEGRO_KEY_PAD_ASTERISK:
			InsertSign( '*' );
			break;
		case ALLEGRO_KEY_PAD_MINUS:
			InsertSign( '-' );
			break;
		case ALLEGRO_KEY_PAD_PLUS:
			InsertSign( '+' );
			break;
		case ALLEGRO_KEY_PAD_DELETE:
			InsertSign( !isShift ? ',' : '.' );	/////////////////////////
			break;
		}
	}
}

void StringToEnter::Update( int keyCode, int action )
{
	switch( action )
	{
	case ACTION_STRING_TO_ENTER_PRESSED:
		cooldownToUseKey[keyCode] = 0.311;
		break;
	case ACTION_STRING_TO_ENTER_HOLDED:
		cooldownToUseKey[keyCode] -= basicWindow->GetDeltaTime();
		if( cooldownToUseKey[keyCode] > 0.0 )
			return;
		else
			cooldownToUseKey[keyCode] = 1.0/13.0;
		break;
	default:
		return;
	}
	
	static bool capsLock = false;
	bool shift = isKeyDown( ALLEGRO_KEY_LSHIFT ) || isKeyDown( ALLEGRO_KEY_RSHIFT );
	
	switch( keyCode )
	{
	case ALLEGRO_KEY_LEFT:
		--currentPosition;
		if( currentPosition < 0 )
			currentPosition = 0;
		break;
	case ALLEGRO_KEY_RIGHT:
		++currentPosition;
		if( currentPosition > str.size() )
			currentPosition = str.size();
		break;
		
	case ALLEGRO_KEY_CAPSLOCK:
		capsLock = !capsLock;
		break;
		
	case ALLEGRO_KEY_PAD_ENTER:
	case ALLEGRO_KEY_ENTER:
		if( shift )
		{
			InsertSign( '\n' );
		}
		else
		{
			if( basicWindow->GetEventResponser() )
			{
				basicWindow->GetEventResponser()->StringToEnterEvent( str );
			}
			Clear();
		}
		break;
		
	case ALLEGRO_KEY_DELETE:
		if( currentPosition < str.size() )
			str.erase( str.begin()+currentPosition );
		break;
	case ALLEGRO_KEY_BACKSPACE:
		if( currentPosition > 0 )
		{
			str.erase( str.begin()+currentPosition-1 );
			--currentPosition;
		}
		break;
		
	case ALLEGRO_KEY_HOME:
		currentPosition = 0;
		break;
	case ALLEGRO_KEY_END:
		currentPosition = str.size();
		break;
		
	default:
		InsertKey( keyCode, capsLock, shift );
	}
}

StringToEnter::StringToEnter()
{
	basicWindow = NULL;
	str = "";
	currentPosition = 0;
}

StringToEnter::~StringToEnter()
{
	basicWindow = NULL;
	str = "";
	currentPosition = 0;
	cooldownToUseKey.clear();
}

#endif

