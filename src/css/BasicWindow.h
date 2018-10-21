
#ifndef BASIC_WINDOW_H
#define BASIC_WINDOW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

/*

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_acodec.h>
*/

/*
	Libraries needed:
		-o test -lm -lpthread -lSM -lICE -lX11 -lXext -lXcursor -lXpm -lXi -lXinerama -lXrandr -lGL -lGLU -lpng -lz -lpulse-simple -lpulse -lasound -lopenal -lfreetype -lallegro_monolith-static
*/

#include <atomic>
#include <thread>

#include <Keyboard.h>
#include <EventResponser.h>
#include <Camera.h>
#include <StringToEnter.h>
#include <TextPrinter.h>

void ParallelThreadFunctionToDraw( class BasicWindow * window );

class BasicWindow
{
private:
	
	bool quitWhenPossible;
	
	glm::mat4 glmProjectionTransform3D;
	glm::mat4 glmProjectionTransform2D;
	
	ALLEGRO_TRANSFORM projection2DTransform;
	ALLEGRO_TRANSFORM projection3DTransform;
	
	ALLEGRO_BITMAP * icon;
	ALLEGRO_DISPLAY * display;
	ALLEGRO_FONT * font;
	float zNear, zFar;
	float deltaTime;
	float skippedTime;
	
	bool lockMouse;
	
	std::atomic<bool> parallelThreadToDrawContinue;
	std::thread parallelThreadToDraw;
	std::atomic<bool> useParallelThreadToDraw;
	
	void Use3DSpace();
	void Use2DSpace();
	
	EventResponser * eventResponser;
	
	friend void ParallelThreadFunctionToDraw( BasicWindow* );
	
protected:
	
	StringToEnter * stringToEnter;
	
public:
	
	glm::mat4 Get3DProjectionTransform() const;
	
	void UseParallelThreadToDraw();
	void ShutDownParallelThreadToDraw();
	bool IsParallelToDrawTickInUse();
	
	float GetSkippedTime();
	
	EventResponser * GetEventResponser();
	void SetEventResponser( EventResponser * eventResponser );
	
	ALLEGRO_DISPLAY * GetDisplay();
	ALLEGRO_BITMAP * GetDisplayBitmap();
	
	bool IsMouseLocked();
	void LockMouse();
	void UnlockMouse();
	void HideMouse();
	void ShowMouse();
	
	float GetDeltaTime();
	
	int GetFontWidth();			// get with al_font_width??
	int GetFontHeight();		// get with al_font_width??
	
	unsigned GetWidth();
	unsigned GetHeight();
	
	ALLEGRO_FONT * GetFont();
	
	virtual void ParallelToDrawTick( const float deltaTime ) = 0;
	
	virtual void Tick( const float deltaTime ) = 0;
	void AlTick();
	void OneLoopFullTick();
	void BeginLoop();
	void QueueQuit();
	
	virtual void Draw3D() = 0;
	virtual void Draw2D() = 0;
	void AlDraw();
	
	void GenerateEvents();
	
	void CreatePerspectiveTransform( float zNear, float zFar );
	
	bool InitAllegro();
	bool InitFont();
	void InitTransforms();
	bool CreateDisplay( const char * windowName, int width, int height, bool fullscreen );
	void LoadIconFromFile( const char * iconFile );
	bool Init( const char * windowName, const char * iconFile, int width, int height, bool fullscreen = false );
	
	void Destroy();
	
	BasicWindow();
	~BasicWindow();
};

#endif

