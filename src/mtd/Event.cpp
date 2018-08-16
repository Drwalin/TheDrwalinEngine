
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

float velocity = 1600.0f;

void Event::MouseMoveEvent( int x, int y, int w, int dx, int dy, int dw )
{
	window->camera->Rotate( btVector3( dy/80.0, dx/80.0, 0.0 ) );
	velocity += float(dw)*10.0f;
	if( velocity < 1.0f )
		velocity = 1.0f;
	return;
}

void Event::KeyPressedEvent( int keyCode )
{
	Object * player = engine->GetObject( "Player" );
	
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	case MOUSE_LEFT:
		engine->AddBox( engine->GetAvailableObjectName("Box"), btVector3(0.5,0.5,0.5), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + window->camera->GetForwardVector() ), true, 200.0 )->GetBody()->setLinearVelocity( window->camera->GetForwardVector() * 16.0 );
		break;
	case MOUSE_RIGHT:
		engine->AddBall( engine->GetAvailableObjectName("Ball"), 0.5, btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + window->camera->GetForwardVector() ), true, 200.0 )->GetBody()->setLinearVelocity( window->camera->GetForwardVector() * 16.0 );
		break;
	case ALLEGRO_KEY_SPACE:
		player->GetBody()->applyCentralImpulse( btVector3( 0, velocity*0.8, 0 ) );
		break;
		/*
	case ALLEGRO_KEY_LCTRL:
		player->GetBody()->applyCentralImpulse( btVector3( 0, -velocity*0.8, 0 ) );
		break;
		*/
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
	Object * player = engine->GetObject( "Player" );
	
	btVector3 vector;
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	case ALLEGRO_KEY_EQUALS:
		velocity += 10.0f;
		break;
	case ALLEGRO_KEY_MINUS:
		velocity -= 10.0f;
		if( velocity < 1.0f )
			velocity = 1.0f;
		break;
	case ALLEGRO_KEY_W:
		player->GetBody()->applyCentralImpulse( window->camera->GetFlatForwardVector() * window->GetDeltaTime() * velocity * 7.0 );
		break;
	case ALLEGRO_KEY_A:
		player->GetBody()->applyCentralImpulse( -window->camera->GetFlatRightVector() * window->GetDeltaTime() * velocity * 7.0 );
		break;
	case ALLEGRO_KEY_S:
		player->GetBody()->applyCentralImpulse( -window->camera->GetFlatForwardVector() * window->GetDeltaTime() * velocity * 7.0 );
		break;
	case ALLEGRO_KEY_D:
		player->GetBody()->applyCentralImpulse( window->camera->GetFlatRightVector() * window->GetDeltaTime() * velocity * 7.0 );
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

