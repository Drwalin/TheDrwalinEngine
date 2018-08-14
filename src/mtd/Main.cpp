


#include <Engine.h>

int main()
{
	Engine * engine = new Engine;
	engine->Init( "Engine 0.0.0", NULL, 800, 600, false );
	
	engine->BeginLoop();
	
	engine->Destroy();
	delete engine;
	
	return 0;
}



