


#include <Engine.h>

int main()
{
	Engine * engine = new Engine;
	engine->Init( "Engine 0.0.0", NULL, 800, 600, false );
	
	
	Model * mdl = engine->GetModel( "./media/AmadeusMap/as_oilrig.obj" );
	Object * map = engine->AddCustom( "TestMap", mdl->MakeStaticTriangleCollisionShape(), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(-20,-10,-20) ), false );
	map->SetModel( mdl );
	for( int i = 0; i < 500; ++i )
		engine->AddBox(  engine->GetAvailableObjectName("Box"), btVector3(0.5,0.5,0.5), btTransform( btQuaternion(btVector3(1,1,1),0), btVector3(-16,20,-24) + btVector3((i/7)%7,i/49,i%7) - btVector3(2.5,2.5,2.5) ), true );
	
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	
	return 0;
}



