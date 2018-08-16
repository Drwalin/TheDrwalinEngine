
#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <Engine.h>

void Engine::PauseSimulation()
{
	pausePhysics = true;
}

void Engine::ResumeSimulation()
{
	pausePhysics = false;
}

int Engine::CalculateNumberOfSimulationsPerFrame( const float deltaTime )
{
	return 0;
	float fps = 1.0 / deltaTime;
	if( fps >= 57.0 )
		return 100;
	if( fps >= 50.0 )
		return 90;
	if( fps >= 40.0 )
		return 60;
	if( fps >= 30.0 )
		return 30;
	if( fps >= 20.0 )
		return 5;
	return 1;
}

void Engine::Tick( const float deltaTime )
{
	if( !pausePhysics )
	{
		world->Tick( deltaTime, CalculateNumberOfSimulationsPerFrame( deltaTime ) );		/////////////////////////////////////////////////////////////////////////
	}
}

Camera * Engine::GetCamera()
{
	return window->camera;
}

std::string Engine::GetAvailableObjectName( std::string name )
{
	if( object.find( name ) == object.end() )
		return name;
	for( int i = 0;; ++i )
		if( object.find( name+std::to_string(i) ) == object.end() )
			return name+std::to_string(i);
	return name;
}

Object * Engine::AddObject( std::string name, btCollisionShape * shape, btTransform transform, std::vector < btScalar > collisionBinaryInfo, int type, bool dynamic, btScalar mass, btVector3 inertia )
{
	if( shape && object.find(name) == object.end() )
	{
		if( dynamic && mass > 0 )
			shape->calculateLocalInertia( mass, inertia );
		else
			mass = 0;
		
		btDefaultMotionState* motionState = new btDefaultMotionState( transform );
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI( mass, motionState, shape, inertia );
		btRigidBody* rigidBody = new btRigidBody( rigidBodyCI );
		world->AddBody( name, rigidBody );
		
		Object * obj = new Object( this, name, rigidBody, collisionBinaryInfo, type );
		object[name] = obj;
		return obj;
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
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::BOX, dynamic, mass, btVector3(0,0,0) );
}

Object * Engine::AddBall( std::string name, btScalar radius, btTransform transform, bool dynamic, btScalar mass )
{
	std::vector < btScalar > collisionBinaryInfo;
	collisionBinaryInfo.resize( 1 );
	collisionBinaryInfo[0] = radius;
	btCollisionShape * shape = collisionShape[collisionBinaryInfo];
	if( shape == NULL )
	{
		shape = new btSphereShape( radius );
		collisionShape[collisionBinaryInfo] = shape;
	}
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::BALL, dynamic, mass, btVector3(0,0,0) );
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
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::CAPSULE, dynamic, mass, btVector3(0,0,0) );
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
	
	return AddObject( name, shape, transform, collisionBinaryInfo, Object::CYLINDER, dynamic, mass, btVector3(0,0,0) );
}

Object * Engine::AddCustom( std::string name, btCollisionShape * collisionShape, btTransform transform, bool dynamic, btScalar mass, btVector3 inertia )
{
	customCollisionShape.push_back( collisionShape );
	return AddObject( name, collisionShape, transform, std::vector<btScalar>(), Object::CUSTOM, dynamic, mass, inertia );
}

Object * Engine::AddCharacter( std::string name, btScalar width, btScalar height, btTransform transform, btScalar mass )
{
	Object * obj =  AddCapsule( name, width/2.0, height, transform, true, mass );
	if( obj )
	{
		obj->GetBody()->setAngularFactor( btVector3( 0, 0.05, 0 ) );
		obj->GetBody()->setActivationState( DISABLE_DEACTIVATION );
		obj->GetBody()->setDamping( 0.99, 0.7 );
		obj->GetBody()->setGravity( world->GetGravity() * 5.0 );
	}
	return obj;
}

void Engine::AttachCameraToObject( std::string name, btVector3 location )
{
	auto it = object.find( name );
	if( it != object.end() )
	{
		cameraParent = it->second;
	}
	else
	{
		cameraParent = NULL;
	}
	GetCamera()->SetPos( location );
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
	Texture * tex = GetTexture("media/Textures/DebugTexturte.png");
	al_draw_indexed_prim( vtx, NULL, tex ? tex->GetBitmapPtr() : NULL, indices, 6*3*2, ALLEGRO_PRIM_TRIANGLE_LIST );
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

bool Engine::SetCustomModel( std::string name, Model * mdl )
{
	auto it = model.find( name );
	if( it == model.end() )
	{
		model[name] = mdl;
		return true;
	}
	return false;
}

Model * Engine::LoadModel( std::string name, int flags, btVector3 arg1, btVector3 origin )
{
	auto it = model.find( name );
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
		if( mdl->LoadFromObj( this, name, flags, arg1, origin ) == false )
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

Texture * Engine::GetTexture( std::string name )
{
	auto it = texture.find( name );
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
	auto it = model.find( name );
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
		if( mdl->LoadFromObj( this, name ) == false )
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

void Engine::DeleteObject( std::string name )
{
	auto it = object.find( name );
	if( it != object.end() )
	{
		if( it->second == cameraParent )
			cameraParent = NULL;
		if( it->second )
		{
			world->RemoveBody( name );
			delete it->second;
		}
		object.erase( it );
	}
}

void Engine::Draw2D()
{
	/*
	output.SetColor( al_map_rgb( 255, 255, 255 ) );
	output.SetWorkSpace( 20, 20, this->GetMaxBorderWidth()-20, this->GetMaxBorderHeight()-20 );
	output.Goto( 1, 1 );
	output.Print( this->GetCurrentStringToEnter().c_str() );
	*/
	
	window->output->SetWorkSpace( 5, 5, 80, 80 );
	window->output->Goto( 5, 5 );
	window->output->SetColor( al_map_rgb( 0, 255, 255 ) );
	window->output->Print( "FPS: " );
	window->output->Print( window->GetSmoothFps() );
	window->output->Print( "\n" );
	window->output->Print( (int)al_get_time() );
	
	window->output->Print( "\n\nCamera position: " );
	window->output->Print( window->camera->GetPos().x() );
	window->output->Print( " : " );
	window->output->Print( window->camera->GetPos().y() );
	window->output->Print( " : " );
	window->output->Print( window->camera->GetPos().z() );
	
	window->output->Print( "\nObjects: " );
	window->output->Print( int(object.size()) );
}

void Engine::Draw3D()
{
	if( cameraParent )
		GetCamera()->SetCameraTransform( cameraParent->GetTransform() );
	
	for( auto it = object.begin(); it != object.end(); ++it )
	{
		if( it->second )
		{
			it->second->Draw();
		}
	}
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
}

void Engine::Destroy()
{
	cameraParent = NULL;
	
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
	cameraParent = NULL;
}

Engine::~Engine()
{
	Destroy();
}

#endif

