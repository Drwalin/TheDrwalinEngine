
#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <Engine.h>

void Engine::Tick( const float deltaTime )
{
	world->Tick( deltaTime, 10 );		/////////////////////////////////////////////////////////////////////////
}

void Engine::Draw2D()
{
	/*
	output.SetColor( al_map_rgb( 255, 255, 255 ) );
	output.SetWorkSpace( 20, 20, this->GetMaxBorderWidth()-20, this->GetMaxBorderHeight()-20 );
	output.Goto( 1, 1 );
	output.Print( this->GetCurrentStringToEnter().c_str() );
	*/
	
	window->output->SetWorkSpace( 5, 5, 20, 20 );
	window->output->Goto( 5, 5 );
	window->output->SetColor( al_map_rgb( 0, 255, 255 ) );
	window->output->Print( "FPS: " );
	window->output->Print( window->GetSmoothFps() );
	window->output->Print( "\n" );
	window->output->Print( (int)al_get_time() );
}

void Engine::DrawBox( ALLEGRO_BITMAP * tex, ALLEGRO_COLOR color, btTransform transform, btVector3 size )
{
	ALLEGRO_COLOR c = al_map_rgb_f(1, 0, 0);
	ALLEGRO_VERTEX vtx[8] = {
		/*   x   y   z   u   v  c  */
		{ -size.x(), -size.y(), -size.z(), 64, 64, color },
		{ -size.x(), -size.y(),  size.z(), 64,  0, color },
		{  size.x(), -size.y(), -size.z(),  0, 64, color },
		{  size.x(), -size.y(),  size.z(), 64,  0, color },
		{ -size.x(),  size.y(), -size.z(), 64, 64, color },
		{ -size.x(),  size.y(),  size.z(), 64,  0, color },
		{  size.x(),  size.y(), -size.z(),  0, 64, color },
		{  size.x(),  size.y(),  size.z(),  0,  0, color },
	};
	
	int indices[] = {
		0, 1, 2,
		1, 2, 3,
		
		4, 5, 6,
		5, 6, 7,
		
		0, 1, 4,
		1, 4, 5,
		
		2, 3, 6,
		3, 6, 7,
		
		0, 2, 4,
		2, 4, 6,
		
		1, 3, 5,
		3, 5, 7
	};
	
	window->camera->SetWorldTransform( transform );
	al_draw_indexed_prim( vtx, NULL, tex, indices, 6*3*2, ALLEGRO_PRIM_TRIANGLE_LIST );
}

void Engine::DrawBall( ALLEGRO_BITMAP * tex, ALLEGRO_COLOR color, btTransform transform, float radian )
{
	radian = sqrt( radian * radian / 3 );
	btVector3 size( radian, radian, radian );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0, 1, 1),  0 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0, 1, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0,-1, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0, 1,-1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0,-1,-1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1, 1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1,-1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1, 1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1,-1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1, 0, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1, 0, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1, 0,-1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( tex, al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1, 0,-1), 30 ), btVector3(0,0,0) ), size );
}

void Engine::Draw3D()
{
	static ALLEGRO_BITMAP * tex = LoadTexture( "./media/bkg.png" );
	
	
	btTransform transform;
	world->object["box"]->getMotionState()->getWorldTransform(transform);
	DrawBox( tex, al_map_rgb(255,255,255), transform, btVector3(5,1,3) );
	
	world->object["ball"]->getMotionState()->getWorldTransform(transform);
	DrawBall( tex, al_map_rgb(255,255,255), transform, 1 );
	
	
	
	{
		ALLEGRO_COLOR c = al_map_rgb_f(1, 1, 1);
		ALLEGRO_VERTEX vtx[4] = {
			//   x   y   z   u   v  c
			{-1000, 0, -1000,       0,       0, c },
			{ 1000, 0, -1000, 64*1024,       0, c },
			{ 1000, 0,  1000, 64*1024, 64*1024, c },
			{-1000, 0,  1000,       0, 64*1024, c },
		};
		
		int indices[6] = {
			0, 1, 3,
			1, 2, 3,
		};
		
		window->camera->SetWorldTransform( btTransform( btQuaternion( btVector3(1,1,1), 0 ), btVector3(0,1,0) ) );
		al_draw_indexed_prim( vtx, NULL, tex, indices, 6, ALLEGRO_PRIM_TRIANGLE_LIST );
	}
	
}

void Engine::BeginLoop()
{
	window->BeginLoop();
}

void Engine::Init( const char * windowName, const char * iconFile, int width, int height, bool fullscreen )
{
	Destroy();
	event = new Event;
	world = new World;
	window = new Window;
	world->Init();
	window->Init( this, windowName, iconFile, width, height, fullscreen );
	window->SetEventResponser( event );
	event->Init();
	event->SetEngine( this );
	
	window->HideMouse();
	window->LockMouse();
	
	
	
	
	world->AddObject( "ball", new btSphereShape(1.0), btTransform( btQuaternion( 0, 0, 0 ), btVector3( 0, 50, 0 ) ), true, 1.0, btVector3(0,0,0) );
	world->AddObject( "box", new btBoxShape( btVector3( 5, 1, 3 ) ), btTransform( btQuaternion( 0, 0, 0 ), btVector3( 1, 60, 0 ) ), true, 1.0, btVector3(0,0,0) );
	world->AddObject( "plane", new btStaticPlaneShape( btVector3(0, 1, 0), 1 ), btTransform( btQuaternion( 0, 0, 0 ), btVector3( 0, 0, 0 ) ) );
}

void Engine::Destroy()
{
	if( window )
	{
		window->Destroy();
		delete window;
		window = NULL;
	}
	
	if( world )
	{
		world->Destroy();
		delete world;
		world = NULL;
	}
	
	if( event )
	{
		delete event;
		event = NULL;
	}
}

Engine::Engine()
{
	event = NULL;
	world = NULL;
	window = NULL;
}

Engine::~Engine()
{
	Destroy();
}

#endif

