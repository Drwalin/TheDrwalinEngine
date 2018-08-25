
#include <ctime>

#include <Engine.h>

#include <Debug.h>

int main()
{
	srand( time( NULL ) );
	
	Engine * engine = new Engine;
	engine->Init( "Engine 0.0.0", NULL, 800, 600, false );
	
	if( !std::ifstream("./media/Models/Sphere.phmesh").good() )
	{
		if( Model::ConvertObjToMesh( "./media/Models/Sphere.obj", true, 0.5, 0.0, true, btVector3(1,1,1) ) == false )
			DEBUG( "Unable to convert" );
	}
	Model * sphere = engine->LoadModel( "./media/Models/Sphere.phmesh" );
	assert( sphere != NULL );
	engine->SetCustomModelName( "Sphere", sphere );
	
	if( !std::ifstream("./media/Models/Crate01.phmesh").good() )
	{
		if( Model::ConvertObjToMesh( "./media/Models/Crate01.obj", true, 0.5, 0.0, true, btVector3(1,1,1) ) == false )
			DEBUG( "Unable to convert" );
	}
	Model * crate01 = engine->LoadModel( "./media/Models/Crate01.phmesh" );
	assert( crate01 != NULL );
	engine->SetCustomModelName( "Crate01", crate01 );
	
	if( !std::ifstream("./media/AmadeusMap/as_oilrig.phmesh").good() )
	{
		if( Model::ConvertObjToMesh( "./media/AmadeusMap/as_oilrig.obj", true, 0.5, 0.0, false ) == false )
			DEBUG( "Unable to convert" );
	}
	Model * mapModel = engine->LoadModel( "./media/AmadeusMap/as_oilrig.phmesh" );
	assert( mapModel != NULL );
	assert( engine->GetCollisionShapeManager() != NULL );
	btCollisionShape * mapShape = engine->GetCollisionShapeManager()->CreateCustomShape( "Map triangle collision mesh", mapModel, CollisionShapeManager::SHAPE_TYPE_TRIANGLE );
	assert( mapShape != NULL );
	Object * map = engine->AddObject( "TestMap", mapShape, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,0,0) ), false );
	map->SetModel( mapModel );
	map->GetBody()->setFriction( 0.65 );
	map->SetScale( btVector3(0.023,0.023,0.023) );
	
	btCollisionShape * crateShape = engine->GetCollisionShapeManager()->CreateCustomShape( "crate01shape", crate01, CollisionShapeManager::SHAPE_TYPE_CONVEX );
	engine->AddObject( engine->GetAvailableObjectName("Box"), crateShape, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,30,0) - btVector3(2.5,2.5,2.5) ), true, 500.0 )->SetModel( crate01 );
	for( int i = 0; i < 125; ++i )
		engine->AddObject( engine->GetAvailableObjectName("Box"), engine->GetCollisionShapeManager()->GetCustomShape( "crate01shape" ), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,20,0) + btVector3((i/5)%5,i/25,i%5) - btVector3(2.5,2.5,2.5) ), true, 500.0 )->SetModel( crate01 );
	
	Object * player = engine->AddCharacter( "Player", 0.6, 1.75, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(22,10,0) ), 75.0 );
	engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
	
	DEBUG( std::string("Loading cpu time: ") + std::to_string(float(clock())/float(CLOCKS_PER_SEC)) );
	DEBUG( std::string("Loading time: ") + std::to_string(al_get_time()) );
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	engine = NULL;
	
	return 0;
}



