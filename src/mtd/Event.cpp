
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
bool running = false;

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
	Object * temp;
	
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
		running = true;
		break;
		
	case ALLEGRO_KEY_SPACE:
		if( player == NULL )
			break;
		player->GetBody()->applyCentralImpulse( btVector3( 0, velocity*0.8, 0 ) );
		break;
	case ALLEGRO_KEY_LCTRL:
		if( player == NULL )
			break;
		player->SetScale( btVector3( 1.0, 0.25, 1.0 ) );
		engine->GetCamera()->SetLocationScale( btVector3( 1.0, 0.25, 1.0 ) );
		player->GetBody()->applyCentralImpulse( btVector3( 0, -velocity * 0.15, 0 ) );
		break;
		
	case MOUSE_LEFT:
//		temp = engine->AddBox( engine->GetAvailableObjectName("Box"), btVector3(0.5,0.5,0.5), btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + window->camera->GetForwardVector() ), true, 200.0 );
//		temp->GetBody()->setLinearVelocity( player->GetBody()->getLinearVelocity() + window->camera->GetForwardVector() * 16.0 );
//		temp->SetModel( engine->GetModel( "Crate01" ) );
		break;
	case MOUSE_RIGHT:
//		temp = engine->AddBall( engine->GetAvailableObjectName("Ball"), 0.5, btTransform( btQuaternion(btVector3(1,1,1),0), window->camera->GetLocation() + window->camera->GetForwardVector() ), true, 200.0 );
//		temp->GetBody()->setLinearVelocity( player->GetBody()->getLinearVelocity() + window->camera->GetForwardVector() * 16.0 );
		break;
	}
}

void Event::KeyReleasedEvent( int keyCode )
{
	Object * player = engine->GetObject( "Player" );
	
	switch( keyCode )
	{
	case ALLEGRO_KEY_ESCAPE:
		window->QueueQuit();
		break;
	case ALLEGRO_KEY_LCTRL:
		if( player == NULL )
			break;
		player->SetScale( btVector3( 1.0, 1.0, 1.0 ) );
		engine->GetCamera()->SetLocationScale( btVector3( 1.0, 1.0, 1.0 ) );
		player->GetBody()->applyCentralImpulse( btVector3( 0, velocity * 0.15, 0 ) );
		break;
		
	case ALLEGRO_KEY_LSHIFT:
		running = false;
		break;
	}
}

float GetMaxVelocity()
{
	return 6.0f + ( running ? 3.0f : 0.0f );
}

void Event::KeyHoldedEvent( int keyCode )
{
	Object * player = engine->GetObject( "Player" );
	Object * temp;
	
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
		if( player == NULL )
			break;
		if( player->GetBody()->getLinearVelocity().dot( window->camera->GetFlatForwardVector() ) < GetMaxVelocity()  )
		{
			player->GetBody()->applyCentralImpulse( window->camera->GetFlatForwardVector() * window->GetDeltaTime() * velocity * 9.0 );
		}
		break;
	case ALLEGRO_KEY_A:
		if( player == NULL )
			break;
		if( player->GetBody()->getLinearVelocity().dot( -window->camera->GetFlatRightVector() ) < GetMaxVelocity() - 0.5f )
		{
			player->GetBody()->applyCentralImpulse( -window->camera->GetFlatRightVector() * window->GetDeltaTime() * velocity * 9.0 );
		}
		break;
	case ALLEGRO_KEY_S:
		if( player == NULL )
			break;
		if( player->GetBody()->getLinearVelocity().dot( -window->camera->GetFlatForwardVector() ) < GetMaxVelocity() - 1.0f )
		{
			player->GetBody()->applyCentralImpulse( -window->camera->GetFlatForwardVector() * window->GetDeltaTime() * velocity * 9.0 );
		}
		break;
	case ALLEGRO_KEY_D:
		if( player == NULL )
			break;
		if( player->GetBody()->getLinearVelocity().dot( window->camera->GetFlatRightVector() ) < GetMaxVelocity() - 0.5f )
		{
			player->GetBody()->applyCentralImpulse( window->camera->GetFlatRightVector() * window->GetDeltaTime() * velocity * 9.0 );
		}
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

