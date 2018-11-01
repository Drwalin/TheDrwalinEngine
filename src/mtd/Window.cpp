
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
	DEBUG(1)
	Destroy();
	DEBUG(2)
	BasicWindow::Init( windowName, iconFile, width, height, fullscreen );
	DEBUG(3)
	output->SetBasicWindow( this );
	DEBUG(4)
	output->ClearWorkspaceBorders();
	DEBUG(5)
	this->engine = engine;
}

void Window::Destroy()
{
	engine = NULL;
	BasicWindow::Destroy();
}

Window::Window()
{
	DEBUG(0)
	engine = NULL;
	DEBUG(1)
	camera = new Camera;
	DEBUG(2)
	output = new TextPrinter;
	DEBUG(3)
	output->SetBasicWindow( this );
}

Window::~Window()
{
	Destroy();
	camera.Delete();
	camera = NULL;
	delete output;
	output = NULL;
	
}

#endif

