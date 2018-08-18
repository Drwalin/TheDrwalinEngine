
#include <ctime>

#include <Engine.h>

#include <Debug.h>

int main()
{
DEBUG(1)
	srand( time( NULL ) );
	
DEBUG(2)
	Engine * engine = new Engine;
DEBUG(3)
	engine->Init( "Engine 0.0.0", NULL, 800, 600, false );
DEBUG(4)
	
	Model * sphere = engine->LoadModel( "./media/Models/Sphere.obj" , Model::CENTER_AABB_MID | Model::RESCALE_TO_SIZE, btVector3(1,1,1) );
	engine->SetCustomModel( "Sphere", sphere );
	
	Model * crate01 = engine->LoadModel( "./media/Models/Crate01.obj" , Model::CENTER_AABB_MID | Model::RESCALE_TO_SIZE, btVector3(1,1,1) );
	engine->SetCustomModel( "Crate01", crate01 );
	
	Model * mdl = engine->LoadModel( "./media/AmadeusMap/as_oilrig.obj", Model::CENTER_AABB_MID | Model::RESCALE_WITH_SCALE, btVector3(0.023,0.023,0.023) );
	Object * map = engine->AddCustom( "TestMap", mdl->MakeStaticTriangleCollisionShape(), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,-10,0) ), false );
	map->SetModel( mdl );
	
	for( int i = 0; i < 100; ++i )
		engine->AddBox( engine->GetAvailableObjectName("Box"), btVector3(0.5,0.5,0.5), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,20,0) + btVector3((i/5)%5,i/25,i%5) - btVector3(2.5,2.5,2.5) ), true, 200.0 )->SetModel( crate01 );
	
	Object * player = engine->AddCharacter( "Player", 0.6, 1.75, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(20,1,0) ), 75.0 );
	engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
	
DEBUG(14)
	engine->BeginLoop();
DEBUG(15)
	
	engine->Destroy();
DEBUG(16)
	delete engine;
DEBUG(17)
	
	return 0;
}



