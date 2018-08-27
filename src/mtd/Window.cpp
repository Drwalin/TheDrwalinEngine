
#ifndef WINDOW_CPP
#define WINDOW_CPP

#include <Window.h>
#include <Engine.h>

void Window::ParallelToDrawTick( const float deltaTime )
{
	if( engine )
		engine->ParallelToDrawTick( deltaTime );
}

void Window::Tick( const float deltaTime )
{
	if( engine )
		engine->Tick( deltaTime );
}

void Window::Draw3D()
{
	if( engine )
		engine->Draw3D();
}

float Window::GetSmoothFps()
{
	static float last_fps = 0;
	static float fps = 0;
	static int n = 0;
	
	if( n == 20 )
	{
		last_fps = fps / float(n);
		n = 0;
		fps = 0;
	}
	
	fps += 1.0 / this->GetDeltaTime();
	++n;
	
	return last_fps;
}

void Window::Draw2D()
{
	if( engine )
		engine->Draw2D();
}

void Window::Init( Engine * engine, const char * windowName, const char * iconFile, int width, int height, bool fullscreen )
{
	Destroy();
	BasicWindow::Init( windowName, iconFile, width, height, fullscreen );
	output->SetBasicWindow( this );
	output->ClearWorkspaceBorders();
	this->engine = engine;
}

void Window::Destroy()
{
	engine = NULL;
	BasicWindow::Destroy();
}

Window::Window()
{
	engine = NULL;
	camera = new Camera;
	output = new TextPrinter;
}

Window::~Window()
{
	Destroy();
	delete camera;
	camera = NULL;
	delete output;
	output = NULL;
	
}

#endif

