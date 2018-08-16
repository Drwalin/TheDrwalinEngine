
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

float velocity = 16.0f;

void Event::MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw )
{
	window->camera->Rotate( btVector3( dy/80.0, dx/80.0, 0.0 ) );
	velocity += float(dw);
	if( velocity < 1.0f )
		velocity = 1.0f;
	return;
}

void Event::KeyPressedEvent( int keyCode )
{
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	case MOUSE_LEFT:
		engine->AddBox( engine->GetAvailableObjectName("Box"), btVector3(0.5,0.5,0.5), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetPos() + window->camera->GetForwardVector() ), true )->GetBody()->setLinearVelocity( window->camera->GetForwardVector() * 16.0 );
		break;
	case MOUSE_RIGHT:
		engine->AddBall( engine->GetAvailableObjectName("Ball"), 0.5, btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetPos() + window->camera->GetForwardVector() ), true )->GetBody()->setLinearVelocity( window->camera->GetForwardVector() * 16.0 );
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
	case ALLEGRO_KEY_EQUALS:
		velocity += 1.0f;
		break;
	case ALLEGRO_KEY_MINUS:
		velocity -= 1.0f;
		if( velocity < 1.0f )
			velocity = 1.0f;
		break;
	case ALLEGRO_KEY_W:
		window->camera->Move( window->camera->GetForwardVector() * window->GetDeltaTime() * velocity );
		break;
	case ALLEGRO_KEY_A:
		window->camera->Move( -window->camera->GetFlatRightVector() * window->GetDeltaTime() * velocity );
		break;
	case ALLEGRO_KEY_S:
		window->camera->Move( -window->camera->GetForwardVector() * window->GetDeltaTime() * velocity );
		break;
	case ALLEGRO_KEY_D:
		window->camera->Move( window->camera->GetFlatRightVector() * window->GetDeltaTime() * velocity );
		break;
	case ALLEGRO_KEY_SPACE:
		window->camera->Move( btVector3( 0, window->GetDeltaTime() * velocity, 0 ) );
		break;
	case ALLEGRO_KEY_LCTRL:
		window->camera->Move( btVector3( 0, -window->GetDeltaTime() * velocity, 0 ) );
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

