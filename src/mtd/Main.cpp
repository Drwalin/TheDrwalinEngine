


#include <Engine.h>

int main()
{
	Engine * engine = new Engine;
	engine->Init( "Engine 0.0.0", NULL, 800, 600, false );
	
	const static int CENTER_NONE = 0;
	const static int CENTER_BARYCENTER = 1;
	const static int CENTER_AABB_MID = 2;
	const static int RESCALE_FULL_SIZE = 4;
	const static int RESCALE_WITH_SCALE = 8;
	
	Model * sphere = engine->LoadModel( "./media/Models/Sphere.obj" , Model::CENTER_AABB_MID | Model::RESCALE_TO_SIZE, btVector3(1,1,1) );
	engine->SetCustomModel( "Sphere", sphere );
	
	Model * mdl = engine->LoadModel( "./media/AmadeusMap/as_oilrig.obj", Model::CENTER_AABB_MID | Model::RESCALE_WITH_SCALE, btVector3(0.023,0.023,0.023) );
	Object * map = engine->AddCustom( "TestMap", mdl->MakeStaticTriangleCollisionShape(), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(-20,-10,-20) ), false );
	map->SetModel( mdl );
	for( int i = 0; i < 10; ++i )
		engine->AddBox( engine->GetAvailableObjectName("Box"), btVector3(0.5,0.5,0.5), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,20,0) + btVector3((i/7)%7,i/49,i%7) - btVector3(2.5,2.5,2.5) ), true, 200.0 );
	
	Object * player = engine->AddCharacter( "Player", 0.6, 1.75, btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(0,40,0) ), 75.0 );
	engine->AttachCameraToObject( "Player", btVector3( 0, 0.8, 0 ) );
	
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	
	return 0;
}



