
#include <ctime>

#include <Engine.h>

#include <Debug.h>

int main()
{
	srand( time( NULL ) );
	
	Engine * engine = new Engine;
	engine->Init( "Engine 0.0.0", NULL, 800, 600, false );
	
	Model * sphere = engine->LoadModel( "./media/Models/Sphere.obj" , Model::CENTER_AABB_MID | Model::RESCALE_TO_SIZE, btVector3(1,1,1) );
	engine->SetCustomModelName( "Sphere", sphere );
	
	Model * crate01 = engine->LoadModel( "./media/Models/Crate01.obj" , Model::CENTER_AABB_MID | Model::RESCALE_TO_SIZE, btVector3(1,1,1) );
	engine->SetCustomModelName( "Crate01", crate01 );
	
	
	Model * mapModel = engine->LoadModel( "./media/AmadeusMap/as_oilrig.obj", Model::CENTER_AABB_MID | Model::RESCALE_WITH_SCALE, btVector3(0.023,0.023,0.023) );
	float beg = al_get_time();
	btCollisionShape * mapShape = engine->GetCollisionShapeManager()->CreateCustomShape( "Map triangle collision mesh", mapModel, CollisionShapeManager::SHAPE_TYPE_TRIANGLE );
	DEBUG( std::string("Map loading time: ")+std::to_string(al_get_time()-beg) );
	Object * map = engine->AddObject( "TestMap", mapShape, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10,0) ), false );
	map->SetModel( mapModel );
	map->GetBody()->setFriction( 0.65 );
	
	
	
	for( int i = 0; i < 100; ++i )
		engine->AddObject( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetBox( btVector3(0.5,0.5,0.5) ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,20,0) + btVector3((i/5)%5,i/25,i%5) - btVector3(2.5,2.5,2.5) ), true, 500.0 )->SetModel( crate01 );
	
	Object * player = engine->AddCharacter( "Player", 0.6, 1.75, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(20,1,0) ), 75.0 );
	engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
	
	DEBUG( std::string("Full init time: ")+std::to_string(al_get_time()) );
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	
	return 0;
}



