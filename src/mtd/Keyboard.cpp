
#ifndef KEYBOARD_CPP
#define KEYBOARD_CPP

#include <Keyboard.h>

ALLEGRO_KEYBOARD_STATE keyboardLastFrame;
ALLEGRO_KEYBOARD_STATE keyboardCurrentFrame;
ALLEGRO_MOUSE_STATE    mouseLastFrame;
ALLEGRO_MOUSE_STATE    mouseCurrentFrame;


bool isKeyDown(int keyCode)
{
	if( keyCode > ALLEGRO_KEY_MAX )
	{
		switch( keyCode )
		{
		case MOUSE_LEFT:
			return bool(mouseCurrentFrame.buttons & 1);
		case MOUSE_RIGHT:
			return bool(mouseCurrentFrame.buttons & 2);
		case MOUSE_MIDDLE:
			return bool(mouseCurrentFrame.buttons & 4);
		}
		return false;
	}
	
    if(al_key_down(&keyboardCurrentFrame, keyCode))
        return true;
    return false;
}

bool wasKeyDown(int keyCode)
{
	if( keyCode > ALLEGRO_KEY_MAX )
	{
		switch( keyCode )
		{
		case MOUSE_LEFT:
			return bool(mouseLastFrame.buttons & 1);
		case MOUSE_RIGHT:
			return bool(mouseLastFrame.buttons & 2);
		case MOUSE_MIDDLE:
			return bool(mouseLastFrame.buttons & 4);
		}
		return false;
	}
	
    if(al_key_down(&keyboardLastFrame, keyCode))
        return true;
    return false;
}

bool isKeyPressed(int keyCode)
{
	if( keyCode > ALLEGRO_KEY_MAX )
	{
		switch( keyCode )
		{
		case MOUSE_LEFT:
			return bool(mouseCurrentFrame.buttons & 1) && (!bool(mouseLastFrame.buttons & 1));
		case MOUSE_RIGHT:
			return bool(mouseCurrentFrame.buttons & 2) && (!bool(mouseLastFrame.buttons & 2));
		case MOUSE_MIDDLE:
			return bool(mouseCurrentFrame.buttons & 4) && (!bool(mouseLastFrame.buttons & 4));
		}
		return false;
	}
	
    if(al_key_down(&keyboardLastFrame, keyCode) == 0 && al_key_down(&keyboardCurrentFrame, keyCode))
        return true;
    return false;
}

bool isKeyReleased(int keyCode)
{
	if( keyCode > ALLEGRO_KEY_MAX )
	{
		switch( keyCode )
		{
		case MOUSE_LEFT:
			return (!bool(mouseCurrentFrame.buttons & 1)) && (bool(mouseLastFrame.buttons & 1));
		case MOUSE_RIGHT:
			return (!bool(mouseCurrentFrame.buttons & 2)) && (bool(mouseLastFrame.buttons & 2));
		case MOUSE_MIDDLE:
			return (!bool(mouseCurrentFrame.buttons & 4)) && (bool(mouseLastFrame.buttons & 4));
		}
		return false;
	}
	
    if(al_key_down(&keyboardLastFrame, keyCode) && al_key_down(&keyboardCurrentFrame, keyCode) == 0)
        return true;
    return false;
}

bool isKeyStillDown(int keyCode)
{
	if( keyCode > ALLEGRO_KEY_MAX )
	{
		switch( keyCode )
		{
		case MOUSE_LEFT:
			return (bool(mouseCurrentFrame.buttons & 1)) && (bool(mouseLastFrame.buttons & 1));
		case MOUSE_RIGHT:
			return (bool(mouseCurrentFrame.buttons & 2)) && (bool(mouseLastFrame.buttons & 2));
		case MOUSE_MIDDLE:
			return (bool(mouseCurrentFrame.buttons & 4)) && (bool(mouseLastFrame.buttons & 4));
		}
		return false;
	}
	
    if(al_key_down(&keyboardLastFrame, keyCode) && al_key_down(&keyboardCurrentFrame, keyCode))
        return true;
    return false;
}

bool isKeyStillUp(int keyCode)
{
	if( keyCode > ALLEGRO_KEY_MAX )
	{
		switch( keyCode )
		{
		case MOUSE_LEFT:
			return (!bool(mouseCurrentFrame.buttons & 1)) && (!bool(mouseLastFrame.buttons & 1));
		case MOUSE_RIGHT:
			return (!bool(mouseCurrentFrame.buttons & 2)) && (!bool(mouseLastFrame.buttons & 2));
		case MOUSE_MIDDLE:
			return (!bool(mouseCurrentFrame.buttons & 4)) && (!bool(mouseLastFrame.buttons & 4));
		}
		return false;
	}
	
    if(al_key_down(&keyboardLastFrame, keyCode) == 0 && al_key_down(&keyboardCurrentFrame, keyCode) == 0)
        return true;
    return false;
}



int GetMouseX()
{
	return mouseCurrentFrame.x;
}

int GetMouseY()
{
	return mouseCurrentFrame.y;
}

int GetMousedX()
{
	return mouseCurrentFrame.x - mouseLastFrame.x;
}

int GetMousedY()
{
	return mouseCurrentFrame.y - mouseLastFrame.y;
}

int GetMouseWheelPos()
{
	return mouseCurrentFrame.z;
}

int GetMouseWheelDelta()
{
	return mouseCurrentFrame.z - mouseLastFrame.z;
}

int GetMouseHorizontalWheelPos()
{
	return mouseCurrentFrame.w;
}

int GetMouseHorizontalWheelDelta()
{
	return mouseCurrentFrame.w - mouseLastFrame.w;
}



void UpdateKeyboard()
{
	keyboardLastFrame = keyboardCurrentFrame;
	mouseLastFrame = mouseCurrentFrame;
	al_get_keyboard_state( &keyboardCurrentFrame );
	al_get_mouse_state( &mouseCurrentFrame );
}

#endif

