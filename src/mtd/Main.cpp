
#include <ctime>

#include <Engine.h>

#include <Debug.h>
#include <SmartPtr.h>

#include "../game/Header.h"

#include "../game/Player.h"

int main()
{
	ConvertMeshes( "media/meshes.list" );
	
	srand( time( NULL ) );
	
	Engine * engine = new Engine;
	engine->Init( "Engine 0.0.0", NULL, 320, 240, true );
	
	LoadMeshes( "media/loadMeshes.list", engine );
	
	
	
	SmartPtr<Model> sphere = engine->GetModel( "Sphere" );
	SmartPtr<Model> crate01 = engine->GetModel( "Crate01" );
	SmartPtr<Model> mapModel = engine->GetModel( /*"Plane" );*/"as_oilrig" );
	assert( mapModel );
	
	SmartPtr<btCollisionShape> mapShape = engine->GetCollisionShapeManager()->CreateCustomShape( "Map triangle collision mesh", mapModel, CollisionShapeManager::SHAPE_TYPE_TRIANGLE );
	assert( mapShape );
	
	
	SmartPtr<Object> map = engine->AddObject<Object>( "TestMap", mapShape, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-30,0) ), false );
	map->SetModel( mapModel );
	map->GetBody()->setFriction( 0.65 );
	map->SetScale( btVector3(0.023,0.023,0.023) );
	
	
	
	if( false )
	{
		auto o = engine->AddObject<Object>( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetBox( btVector3(1.5,1.5,1.5) )/*engine->GetCollisionShapeManager()->GetCustomShape( "crate01shape" )*/, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10.3,0) ), false );
		o->SetModel( crate01 );
		o->SetScale( btVector3( 30, 3, 30 ) );
	}
	
	
	
	engine->AddObject<Object>( engine->GetAvailableObjectName("Brick"), engine->GetCollisionShapeManager()->CreateCustomShape( engine->GetCollisionShapeManager()->GetFirstAvailableName("Brick"), engine->GetModel("Brick"), CollisionShapeManager::SHAPE_TYPE_CONVEX ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,30,0) ), true, 10 )->SetModel( engine->GetModel("Brick") );
	engine->AddObject<Object>( engine->GetAvailableObjectName("ConcreetBrick"), engine->GetCollisionShapeManager()->CreateCustomShape( engine->GetCollisionShapeManager()->GetFirstAvailableName("ConcreetBrick"), engine->GetModel("ConcreetBrick"), CollisionShapeManager::SHAPE_TYPE_CONVEX ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,15,0) ), true, 20 )->SetModel( engine->GetModel("ConcreetBrick") );
	engine->AddObject<Object>( engine->GetAvailableObjectName("m4a1"), engine->GetCollisionShapeManager()->CreateCustomShape( engine->GetCollisionShapeManager()->GetFirstAvailableName("m4a1"), engine->GetModel("m4a1"), CollisionShapeManager::SHAPE_TYPE_CONVEX ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,40,0) ), true, 10 )->SetModel( engine->GetModel("m4a1") );
	
	
	
	
	for( int i = 0; i < 0; ++i )
	{
		auto o = engine->AddObject<Object>( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetBox( btVector3(0.5,0.5,0.5) )/*engine->GetCollisionShapeManager()->GetCustomShape( "crate01shape" )*/, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,10.3,0) + btVector3(0,(i%12)*0.3,((i/12))+(float(i%2)/2.0)) ), true, 50.0 );
		o->SetModel( crate01 );
		o->SetScale( btVector3( 0.6, 0.3, 1 ) );
		o->GetBody()->setFriction( 0.9 );
	}
	
	SmartPtr<Object> player = engine->AddCharacter<Player>( "Player", 0.6, 1.75, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(22,10,0) ), 15.0 );
	engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
	((Character*)(player.GetPtr()))->SetCamera( engine->GetCamera() );
	
	
	
	
	DEBUG( std::string("Loading cpu time: ") + std::to_string(float(clock())/float(CLOCKS_PER_SEC)) );
	DEBUG( std::string("Loading time: ") + std::to_string(al_get_time()) );
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	engine = NULL;
	
	return 0;
}



