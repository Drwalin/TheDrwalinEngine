
#ifndef EVENT_CPP
#define EVENT_CPP

#include <Event.h>
#include <Window.h>
#include <Engine.h>
#include "../game/Character.h"
#include <irrlicht\irrlicht.h>

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
	std::shared_ptr<Object> player = engine->GetObject( std::string("Player") );
	if( player )
	{
		Character * character = dynamic_cast < Character* > ( (Object*)(player.get()) );
		if( character )
		{
			character->EventRotateCameraBy( btVector3( float(dy)/160.0, float(dx)/160.0, 0.0 ) );
		}
	}
}





void Event::KeyPressedEvent( int keyCode )
{
	std::shared_ptr<Object> player = engine->GetObject( std::string("Player") );
	std::shared_ptr<Object> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( ((Object*)(player.get())) );
	
	switch( keyCode )
	{
	case irr::KEY_ESCAPE:
		window->QueueQuit();
		break;
		
	case irr::KEY_KEY_T:
		fprintf( stderr, "\n Number of objects: %i ", int(engine->object.size()) );
		break;
		
	case irr::KEY_KEY_P:
		if( engine->GetWindow()->IsMouseLocked() )
			engine->GetWindow()->UnlockMouse();
		else
			engine->GetWindow()->LockMouse();
		break;
		
	case irr::KEY_LSHIFT:
		if( character )
			character->EventBeginRun();
		break;
	case irr::KEY_LCONTROL:
		if( character )
			character->EventCrouch();
		break;
	case irr::KEY_MENU:
		if( character )
			character->EventBeginStravage();
		break;
		
	case irr::KEY_LBUTTON:
		temp = engine->AddObject<Object>( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetCustomShape( "crate01shape__1331_" ), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + character->GetForwardVector()/*window->camera->GetForwardVector()*/ ), true, 20.0, engine->GetModel( "Crate01" )->GetInertia() );
		if( temp )
		{
			temp->GetBody()->setLinearVelocity( player->GetBody()->getLinearVelocity() + character->GetForwardVector()/*window->camera->GetForwardVector()*/ * 16.0 );
			temp->SetModel( engine->GetModel( "Crate01" ) );
			temp->SetScale( btVector3( 0.5, 0.5, 0.5 ) );
		}
		else
		{
			DEBUG("Couldn't spawn new object");
		}
		break;
	case irr::KEY_RBUTTON:
		temp = engine->AddObject<Object>( engine->GetAvailableObjectName("Ball"), engine->GetCollisionShapeManager()->GetBall( 0.5 ), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + window->camera->GetForwardVector() ), true, 20.0 );
		if( temp )
		{
			temp->GetBody()->setLinearVelocity( player->GetBody()->getLinearVelocity() + window->camera->GetForwardVector() * 16.0 );
			temp->SetModel( engine->GetModel( "Sphere" ) );
		}
		else
		{
			DEBUG("Couldn't spawn new object");
		}
		break;
	}
}

void Event::KeyReleasedEvent( int keyCode )
{
	std::shared_ptr<Object> player = engine->GetObject( std::string("Player") );
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Object*)(player.get()) );
	
	switch( keyCode )
	{
	case irr::KEY_ESCAPE:
		window->QueueQuit();
		break;
		
		
	case irr::KEY_LSHIFT:
		if( character )
			character->EventStopRun();
		break;
	case irr::KEY_LCONTROL:
		if( character )
			character->EventStandUp();
		break;
	case irr::KEY_MENU:
		if( character )
			character->EventStopStravage();
		break;
	}
	
	KeyHoldedEvent( keyCode );
}

void Event::KeyHoldedEvent( int keyCode )
{
	std::shared_ptr<Object> player = engine->GetObject( std::string("Player") );
	std::shared_ptr<Object> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Object*)(player.get()) );
	
	btVector3 vector;
	
	switch( keyCode )
	{
	case irr::KEY_MBUTTON:
		if( character )
		{
			temp = engine->GetObject( std::string("Box") );
			if( temp )
			{
				character->EventRotateCameraToLookAtPoint( temp->GetLocation(), true );
			}
		}
		break;
		
	case irr::KEY_SPACE:
		if( character )
			character->EventJump();
		break;
		
	case irr::KEY_ESCAPE:
		window->QueueQuit();
		break;
		
		/*
	case irr::KEY_LBUTTON:
		begin = engine->GetCamera()->GetLocation();
		end = begin + ( engine->GetCamera()->GetForwardVector() * 100.0 );
		temp = engine->RayTrace( begin, end, Engine::RayTraceChannel::COLLIDING, point, normal, { player } );
		if( temp )
		{
			temp->ApplyImpactDamage( 0, 100.0 * engine->GetDeltaTime(), engine->GetCamera()->GetForwardVector(), point, normal );
		}
		break;
		*/
		
		
	case irr::KEY_BACK:
	case irr::KEY_DELETE:
		if( keyCode == irr::KEY_DELETE )
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
		
	case irr::KEY_UP:
		if( character )
			character->EventRotateCameraBy( btVector3( -window->GetDeltaTime(), 0.0, 0.0 ) * 2.0 );
		break;
	case irr::KEY_DOWN:
		if( character )
			character->EventRotateCameraBy( btVector3( window->GetDeltaTime(), 0.0, 0.0 ) * 2.0 );
		break;
	case irr::KEY_RIGHT:
		if( character )
			character->EventRotateCameraBy( btVector3( 0.0, window->GetDeltaTime(), 0.0 ) * 2.0 );
		break;
	case irr::KEY_LEFT:
		if( character )
			character->EventRotateCameraBy( btVector3( 0.0, -window->GetDeltaTime(), 0.0 ) * 2.0 );
		break;
		
	case irr::KEY_KEY_W:
		if( character )
			character->EventMoveInDirection( character->GetFlatForwardVector(), true );
		break;
	case irr::KEY_KEY_A:
		if( character )
			character->EventMoveInDirection( character->GetFlatLeftVector(), true );
		break;
	case irr::KEY_KEY_S:
		if( character )
			character->EventMoveInDirection( -character->GetFlatForwardVector(), true );
		break;
	case irr::KEY_KEY_D:
		if( character )
			character->EventMoveInDirection( -character->GetFlatLeftVector(), true );
		break;
	}
}

void Event::StringToEnterEvent( std::string str )
{
	fprintf( stderr, "\n Input string: \"%s\"", str.c_str() );
	
	std::shared_ptr<Object> player = engine->GetObject( std::string("Player") );
	std::shared_ptr<Object> temp;
	btVector3 begin, end, point, normal;
	Character * character = NULL;
	if( player )
		character = dynamic_cast < Character* > ( (Object*)(player.get()) );
	
	if( str == "Rel" )
	{
		fprintf( stderr, "\n StringToEnterEvent( %s ); ", str.c_str() );
		engine->GetCamera()->SetRotation( btVector3( 0, 0, 0 ) );
		if( character )
		{
			character->SetCameraRotation( btVector3( 0, 0, 0 ) );
		}
	}
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

