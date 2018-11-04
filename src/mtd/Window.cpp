
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

void Window::Init( Engine * engine, const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen )
{
	DEBUG(1)
	Destroy();
	DEBUG(2)
	BasicWindow::Init( windowName, iconFile, width, height, eventResponser, fullscreen );
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
	camera = std::shared_ptr<Camera>( new Camera );
	DEBUG(2)
	output = new TextPrinter;
	DEBUG(3)
	output->SetBasicWindow( this );
}

Window::~Window()
{
	Destroy();
	camera.reset();
	camera = NULL;
	delete output;
	output = NULL;
	
}

#endif

