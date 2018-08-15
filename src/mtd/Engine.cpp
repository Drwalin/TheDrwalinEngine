
#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <Engine.h>

void PauseSimulation()
{
	pausePhysics = true;
}

void ResumeSimulation()
{
	pausePhysics = false;
}

void Engine::Tick( const float deltaTime )
{
	if( !pausePhysics )
	{
		world->Tick( deltaTime, 10 );		/////////////////////////////////////////////////////////////////////////
	}
}

Camera * Engine::GetCamera()
{
	return window->camera;
}

Object * Engine::AddObject( std::string name, btCollisionShape * shape, btTransform transform, std::vector < btScalar > collisionBinaryInfo, int type, bool dynamic, btScalar mass, btVector3 inertia )
{
	if( shape && object.find(name) == object.end() )
	{
		if( dynamic && mass > 0 )
		{
			shape->calculateLocalInertia( mass, inertia );
		}
		else
		{
			mass = 0;
		}
		
		btDefaultMotionState* motionState = new btDefaultMotionState( transofrm );
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI( mass, motionState, shape, inertia );
		btRigidBody* rigidBody = new btRigidBody( rigidBodyCI );
		AddBody( name, rigidBody );
		
		object[name] = new Object( this, name, rigidBody, collisionBinary );
	}
	return NULL;
}

Object * Engine::AddBox( std::string name, btVector3 size, btTransform transform, bool dynamic, btScalar mass )
{
	std::vector < btScalar > collisionBinaryInfo;
	collisionBinaryInfo.resize( 3 );
	collisionBinaryInfo[0] = size.x();
	collisionBinaryInfo[1] = size.y();
	collisionBinaryInfo[2] = size.z();
	btCollisionShape * shape = collisionShape[collisionBinaryInfo];
	if( shape == NULL )
	{
		shape = new btBoxShape( size );
		collisionShape[collisionBinaryInfo] = shape;
	}
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::BOX, dynamic, mass, btVector(0,0,0) );
}

Object * Engine::AddBall( std::string name, btScalar radius, btTransform transform, bool dynamic, btScalar mass )
{
	std::vector < btScalar > collisionBinaryInfo;
	collisionBinaryInfo.resize( 1 );
	collisionBinaryInfo[0] = radius;
	btCollisionShape * shape = collisionShape[collisionBinaryInfo];
	if( shape == NULL )
	{
		shape = new btBallShape( radius );
		collisionShape[collisionBinaryInfo] = shape;
	}
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::BALL, dynamic, mass, btVector(0,0,0) );
}

Object * Engine::AddCapsule( std::string name, btScalar radius, btScalar height, btTransform transform, bool dynamic, btScalar mass )
{
	height;
	std::vector < btScalar > collisionBinaryInfo;
	collisionBinaryInfo.resize( 2 );
	collisionBinaryInfo[0] = radius;
	collisionBinaryInfo[1] = height;
	btCollisionShape * shape = collisionShape[collisionBinaryInfo];
	if( shape == NULL )
	{
		shape = new btCapsuleShape( radius, height/2.0 );
		collisionShape[collisionBinaryInfo] = shape;
	}
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::CAPSULE, dynamic, mass, btVector(0,0,0) );
}

Object * Engine::AddCylinder( std::string name, btScalar radius, btScalar height, btTransform transform, bool dynamic, btScalar mass )
{
	std::vector < btScalar > collisionBinaryInfo;
	collisionBinaryInfo.resize( 4 );
	collisionBinaryInfo[0] = radius;
	collisionBinaryInfo[1] = height;
	collisionBinaryInfo[2] = radius;
	collisionBinaryInfo[3] = height;
	btCollisionShape * shape = collisionShape[collisionBinaryInfo];
	if( shape == NULL )
	{
		shape = new btCylinderShape( btVector3(radius,height/2.0,radius) );
		collisionShape[collisionBinaryInfo] = shape;
	}
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::CYLINDER, dynamic, mass, btVector(0,0,0) );
}

Object * Engine::AddCustom( std::string name, btCollisionShape * collisionShape, btTransform transform, bool dynamic, btScalar mass, btVector3 inertia )
{
	customCollisionShape.push_back( collisionShape );
	return AddObject( name, collisionShape, transform, std::vector<btScalar>(), CUSTOM, dynamic, mass, inertia );
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

void Engine::DrawBox( ALLEGRO_COLOR color, btTransform transform, btVector3 size )
{
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
	al_draw_indexed_prim( vtx, NULL, GetTexture("media/DebugTexturte.png"), indices, 6*3*2, ALLEGRO_PRIM_TRIANGLE_LIST );
}

void Engine::DrawBall( ALLEGRO_COLOR color, btTransform transform, float radian )
{
	radian = sqrt( radian * radian / 3 );
	btVector3 size( radian, radian, radian );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0, 1, 1),  0 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0, 1, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0,-1, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0, 1,-1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 0,-1,-1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1, 1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1,-1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1, 1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1,-1, 0), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1, 0, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1, 0, 1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3( 1, 0,-1), 30 ), btVector3(0,0,0) ), size );
	DrawBox( al_map_rgb(255,255,255), transform*btTransform( btQuaternion( btVector3(-1, 0,-1), 30 ), btVector3(0,0,0) ), size );
}

Texture * Engine::GetTexture( std::string name )
{
	auto it = texture.find( std );
	if( it != texture.end() )
	{
		if( it->second )
			return it->second;
		else
			texture.erase( it );
	}
	else
	{
		Texture * tex = new Texture;
		if( tex->Load( name, Texture::LINEAR | Texture::MIPMAP ) == false )
		{
			delete tex;
			return NULL;
		}
		else
		{
			texture[name] = tex;
			return tex;
		}
	}
	return NULL;
}

Model * Engine::GetModel( std::string name )
{
	auto it = model.find( std );
	if( it != model.end() )
	{
		if( it->second )
			return it->second;
		else
			model.erase( it );
	}
	else
	{
		Model * mdl = new Model;
		if( mdl->Load( this, name ) == false )
		{
			delete mdl;
			return NULL;
		}
		else
		{
			model[name] = mdl;
			return mdl;
		}
	}
	return NULL;
}

Object * Engine::GetObject( std::string name )
{
	auto it = object.find( name );
	if( it != object.end() )
	{
		if( it->second )
		{
			return it->second;
		}
		else
		{
			object.erase( it );
		}
	}
	return NULL;
}

void Engine::Draw3D()
{
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->secod )
		{
			it->second->Draw();
		}
	}
	
	/*
	
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
	*/
}

void Engine::BeginLoop()
{
	pausePhysics = false;
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
	
	for( auto it = collisionShape.begin(); it != collisionShape.end(); ++it )
		delete it->second;
	collisionShape.clear();
	
	for( int i = 0; i < customCollisionShape.size(); ++i )
		delete customCollisionShape[i];
	customCollisionShape.clear();
	
	pausePhysics = false;
}

Engine::Engine()
{
	event = NULL;
	world = NULL;
	window = NULL;
	pausePhysics = true;
}

Engine::~Engine()
{
	Destroy();
}

#endif

