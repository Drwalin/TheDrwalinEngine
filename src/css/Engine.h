
#ifndef ENGINE_H
#define ENGINE_H

#include <Event.h>
#include <World.h>
#include <Window.h>

#include <map>
#include <vector>

class Engine
{
private:
	
	World * world;
	Window * window;
	Event * event;
	
	
	
	
	void DrawBox( ALLEGRO_BITMAP * tex, ALLEGRO_COLOR color, btTransform transform, btVector3 size );
	void DrawBall( ALLEGRO_BITMAP * tex, ALLEGRO_COLOR color, btTransform transform, float radian );
	
public:
	
	void Tick( const float deltaTime );
	
	void Draw2D();
	void Draw3D();
	
	void Init( const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	void BeginLoop();
	
	void Destroy();
	
	Engine();
	~Engine();
};

#endif

