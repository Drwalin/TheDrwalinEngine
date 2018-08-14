
#ifndef KEYBOARD_H
#define KEYBOARD_H

// for key id's:
#include <allegro5/keycodes.h>
#include <allegro5/allegro.h>

#define MOUSE_FIRST_BUTTON MOUSE_LEFT
enum MouseButtons
{
	MOUSE_LEFT = ALLEGRO_KEY_MAX+311,
	MOUSE_RIGHT = ALLEGRO_KEY_MAX+312,
	MOUSE_MIDDLE = ALLEGRO_KEY_MAX+313,
	MOUSE_AFTER_LAST_BUTTON = ALLEGRO_KEY_MAX+314
};


bool isKeyDown( int keyCode );
bool wasKeyDown( int keyCode );
bool isKeyPressed(int keyCode);
bool isKeyReleased(int keyCode);
bool isKeyStillDown(int keyCode);
bool isKeyStillUp(int keyCode);



int GetMouseX();
int GetMouseY();
int GetMousedX();
int GetMousedY();
int GetMouseWheelPos();
int GetMouseWheelDelta();
int GetMouseHorizontalWheelPos();
int GetMouseHorizontalWheelDelta();



void UpdateKeyboard();

#endif

