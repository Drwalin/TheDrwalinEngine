
#ifndef EVENT_CPP
#define EVENT_CPP

#include <Event.h>
#include <Window.h>
#include <Engine.h>

void Event::SetEngine( class Engine * engine )
{
	this->engine = engine;
}

void Event::Init()
{
	window = (Window*)( GetBasicWindow() );
}

void Event::MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw )
{
	window->camera->Rotate( btVector3( dy/80.0, dx/80.0, 0.0 ) );
	return;
}

void Event::KeyPressedEvent( int keyCode )
{
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	}
}

void Event::KeyReleasedEvent( int keyCode )
{
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	}
}

void Event::KeyHoldedEvent( int keyCode )
{
	btVector3 vector;
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	case ALLEGRO_KEY_W:
		window->camera->Move( window->camera->GetForwardVector() * window->GetDeltaTime() * 5 );
		break;
	case ALLEGRO_KEY_A:
		window->camera->Move( -window->camera->GetFlatRightVector() * window->GetDeltaTime() * 5 );
		break;
	case ALLEGRO_KEY_S:
		window->camera->Move( -window->camera->GetForwardVector() * window->GetDeltaTime() * 5 );
		break;
	case ALLEGRO_KEY_D:
		window->camera->Move( window->camera->GetFlatRightVector() * window->GetDeltaTime() * 5 );
		break;
	case ALLEGRO_KEY_SPACE:
		window->camera->Move( btVector3( 0, window->GetDeltaTime() * 5, 0 ) );
		break;
	case ALLEGRO_KEY_LCTRL:
		window->camera->Move( btVector3( 0, -window->GetDeltaTime() * 5, 0 ) );
		break;
	}
}

void Event::StringToEnterEvent( std::string str )
{
	return;
}

Event::Event()
{
	window = NULL;
}

Event::~Event()
{
	window = NULL;
}

#endif

