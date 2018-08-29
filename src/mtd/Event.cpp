
#ifndef EVENT_CPP
#define EVENT_CPP

#include <Event.h>
#include <Window.h>
#include <Engine.h>
#include "../game/Character.h"

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
	SmartPtr<Object> player = engine->GetObject( "Player" );
	if( player )
	{
		Character * character = dynamic_cast < Character* > ( (Object*)(player.GetPtr()) );
		if( character )
		{
			character->EventRotateCameraBy( btVector3( dy/80.0, dx/80.0, 0.0 ) );
		}
	}
}





void Event::KeyPressedEvent( int keyCode )
{
	SmartPtr<Object> player = engine->GetObject( "Player" );
	SmartPtr<Object> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( ((Object*)(player.GetPtr())) );
	
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
		
	case ALLEGRO_KEY_P:
		if( engine->GetWindow()->IsMouseLocked() )
			engine->GetWindow()->UnlockMouse();
		else
			engine->GetWindow()->LockMouse();
		break;
		
	case ALLEGRO_KEY_LSHIFT:
		if( character )
			character->EventBeginRun();
		break;
		
	case ALLEGRO_KEY_SPACE:
		if( character )
			character->EventJump();
		break;
	case ALLEGRO_KEY_LCTRL:
		if( character )
			character->EventCrouch();
		break;
		
	case MOUSE_LEFT:
		temp = engine->AddObject<Object>( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetBox( btVector3(0.5,0.5,0.5) ), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + character->GetForwardVector()/*window->camera->GetForwardVector()*/ ), true, 20.0 );
		temp->GetBody()->setLinearVelocity( player->GetBody()->getLinearVelocity() + character->GetForwardVector()/*window->camera->GetForwardVector()*/ * 16.0 );
		temp->SetModel( engine->GetModel( "Crate01" ) );
		break;
	case MOUSE_RIGHT:
		temp = engine->AddObject<Object>( engine->GetAvailableObjectName("Ball"), engine->GetCollisionShapeManager()->GetBall( 0.5 ), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + window->camera->GetForwardVector() ), true, 20.0 );
		temp->GetBody()->setLinearVelocity( player->GetBody()->getLinearVelocity() + window->camera->GetForwardVector() * 16.0 );
		temp->SetModel( engine->GetModel( "Sphere" ) );
		break;
	}
}

void Event::KeyReleasedEvent( int keyCode )
{
	SmartPtr<Object> player = engine->GetObject( "Player" );
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Object*)(player.GetPtr()) );
	
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
		
		
	case ALLEGRO_KEY_LSHIFT:
		if( character )
			character->EventStopRun();
		break;
	case ALLEGRO_KEY_LCTRL:
		if( character )
			character->EventStandUp();
		break;
	}
}

void Event::KeyHoldedEvent( int keyCode )
{
	SmartPtr<Object> player = engine->GetObject( "Player" );
	SmartPtr<Object> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Object*)(player.GetPtr()) );
	
	btVector3 vector;
	
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
		
		/*
	case MOUSE_LEFT:
		begin = engine->GetCamera()->GetLocation();
		end = begin + ( engine->GetCamera()->GetForwardVector() * 100.0 );
		temp = engine->RayTrace( begin, end, Engine::RayTraceChannel::COLLIDING, point, normal, { player } );
		if( temp )
		{
			temp->ApplyImpactDamage( 0, 100.0 * engine->GetDeltaTime(), engine->GetCamera()->GetForwardVector(), point, normal );
		}
		break;
		*/
		
		
	case ALLEGRO_KEY_BACKSPACE:
	case ALLEGRO_KEY_DELETE:
		if( keyCode == ALLEGRO_KEY_DELETE )
		{
			begin = engine->GetCamera()->GetLocation();
			end = begin + ( engine->GetCamera()->GetForwardVector() * 100.0 );
		}
		else
		{
			begin = btVector3(0,11.2,0);
			end = btVector3(0,11.2,300);
		}
		temp = engine->RayTrace( begin, end, Engine::RayTraceChannel::COLLIDING, point, normal, { player } );
		if( temp )
		{
			if( temp->GetName() != "TestMap" && temp->GetName() != "Box" )
			{
				if( temp->GetBody() )
					temp->GetBody()->activate();
				engine->QueueObjectToDestroy( temp->GetName() );
			}
		}
		break;
		
	case ALLEGRO_KEY_UP:
		if( character )
			character->EventRotateCameraBy( btVector3( -window->GetDeltaTime(), 0.0, 0.0 ) * 2.0 );
		break;
	case ALLEGRO_KEY_DOWN:
		if( character )
			character->EventRotateCameraBy( btVector3( window->GetDeltaTime(), 0.0, 0.0 ) * 2.0 );
		break;
	case ALLEGRO_KEY_RIGHT:
		if( character )
			character->EventRotateCameraBy( btVector3( 0.0, window->GetDeltaTime(), 0.0 ) * 2.0 );
		break;
	case ALLEGRO_KEY_LEFT:
		if( character )
			character->EventRotateCameraBy( btVector3( 0.0, -window->GetDeltaTime(), 0.0 ) * 2.0 );
		break;
		
	case ALLEGRO_KEY_W:
		if( character )
			character->EventMoveInDirection( character->GetFlatForwardVector(), true );
		break;
	case ALLEGRO_KEY_A:
		if( character )
			character->EventMoveInDirection( character->GetFlatLeftVector(), true );
		break;
	case ALLEGRO_KEY_S:
		if( character )
			character->EventMoveInDirection( -character->GetFlatForwardVector(), true );
		break;
	case ALLEGRO_KEY_D:
		if( character )
			character->EventMoveInDirection( -character->GetFlatLeftVector(), true );
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

