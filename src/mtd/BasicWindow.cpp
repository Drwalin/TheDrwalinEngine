
#ifndef BASIC_WINDOW_CPP
#define BASIC_WINDOW_CPP

#include <BasicWindow.h>
#include <Debug.h>
#include <StringToEnter.h>
#include <EventReceiverIrrlicht.h>

void ParallelThreadFunctionToDraw( BasicWindow * window )
{
	if( window )
	{
		while( window->IsParallelToDrawTickInUse() )
		{
			while( window->parallelThreadToDrawContinue.load() == false )
			{
				if( window->IsParallelToDrawTickInUse() == false )
					return;
				//al_rest( 0.001 );
			}
			
			window->ParallelToDrawTick( window->GetDeltaTime() );
			
			window->parallelThreadToDrawContinue.store( false );
		}
	}
}

TimeCounter BasicWindow::GetEventGenerationTime() const
{
	return eventsTime;
}

TimeCounter BasicWindow::GetWholeDrawTime() const
{
	return wholeDrawTime;
}

TimeCounter BasicWindow::GetSkippedTime() const
{
	return skippedTime;
}

bool BasicWindow::IsParallelToDrawTickInUse()
{
	return useParallelThreadToDraw.load();
}

void BasicWindow::UseParallelThreadToDraw()
{
	if( useParallelThreadToDraw.load() == false )
	{
		parallelThreadToDrawContinue.store( false );
		useParallelThreadToDraw.store( true );
		parallelThreadToDraw = std::thread( ParallelThreadFunctionToDraw, this );
	}
}

void BasicWindow::ShutDownParallelThreadToDraw()
{
	if( useParallelThreadToDraw.load() == true )
	{
		useParallelThreadToDraw.store( false );
		parallelThreadToDraw.join();
	}
}

unsigned BasicWindow::GetWidth()
{
	return screenResolution.Width;
}

unsigned BasicWindow::GetHeight()
{
	return screenResolution.Height;
}

bool BasicWindow::IsMouseLocked()
{
	return lockMouse;
}

void BasicWindow::LockMouse()
{
	lockMouse = true;
}

void BasicWindow::UnlockMouse()
{
	lockMouse = false;
}

void BasicWindow::HideMouse()
{
	device->getCursorControl()->setVisible( false );
}

void BasicWindow::ShowMouse()
{
	device->getCursorControl()->setVisible( true );
}

float BasicWindow::GetDeltaTime()
{
	return deltaTime;
}

bool BasicWindow::Init( const char * windowName, const char * iconFile, int width, int height, EventResponser * eventResponser, bool fullscreen )
{
	Destroy();
	
	this->eventResponser = eventResponser;
	this->eventResponser->SetBasicWindow( this );
	eventIrrlichtReceiver = new EventReceiverIrrlicht( eventResponser, this );
	
	screenResolution = irr::core::dimension2du( width, height );
	device = irr::createDevice( irr::video::EDT_OPENGL, screenResolution, 16, fullscreen, false, false, eventIrrlichtReceiver );
	
	videoDriver = device->getVideoDriver();
	sceneManager = device->getSceneManager();
	gui = device->getGUIEnvironment();
	
	stringToEnter->SetBasicWindow( this );
	
	return true;
}

void BasicWindow::Destroy()
{
	ShutDownParallelThreadToDraw();
	
	if( device )
		device->drop();
	
	quitWhenPossible = false;
	
	deltaTime = 0.0;
	
	lockMouse = false;
	
	eventResponser = NULL;
}

void BasicWindow::OneLoopFullTick()
{
	static float beginTime = (float(clock())/1000.0f);
	
	deltaTime = (float(clock())/1000.0f) - beginTime;
	beginTime = (float(clock())/1000.0f);
	
	if( deltaTime < 0.0001 )
		deltaTime = 0.0001;
	else if( deltaTime > 0.3 )
		deltaTime = 0.3;
	
	AlTick();
	
	/*
	if( al_get_time() - beginTime < 1.0/60.0 )
	{
		skippedTime.SubscribeStart();
		al_rest( skippedTime );
		skippedTime.SubscribeEnd();
	}
	*/
}

void BasicWindow::BeginLoop()
{
	quitWhenPossible = false;
	while( !quitWhenPossible && device->run() )
		OneLoopFullTick();
}

void BasicWindow::QueueQuit()
{
	quitWhenPossible = true;
}

void BasicWindow::Draw()
{
	wholeDrawTime.SubscribeStart();
	
	if( IsParallelToDrawTickInUse() )
	{
		DEBUG(0)
		parallelThreadToDrawContinue.store( true );
	}
	
	videoDriver->beginScene( true, true, irr::video::SColor(255,16,32,64) );
	sceneManager->drawAll();
	gui->drawAll();
	videoDriver->endScene();
	
	if( IsParallelToDrawTickInUse() )
	{
		DEBUG(1)
		while( parallelThreadToDrawContinue.load() )
		{
		//	al_rest( 0.001 );
		}
	}
	
	wholeDrawTime.SubscribeStart();
}


void BasicWindow::AlTick()
{
	//UpdateKeyboard();
	
	/*
	if( lockMouse )
	{
		al_set_mouse_xy( display, al_get_bitmap_width(al_get_backbuffer(display)) / 2, al_get_bitmap_height(al_get_backbuffer(display)) / 2 );
		mouseLastFrame.x = al_get_bitmap_width(al_get_backbuffer(display)) / 2;
		mouseLastFrame.y = al_get_bitmap_height(al_get_backbuffer(display)) / 2;
	}
	*/
	
	GenerateEvents();
	
	Tick( deltaTime );
	
	Draw();
}

void BasicWindow::GenerateEvents()
{
	eventsTime.SubscribeStart();
	
	eventIrrlichtReceiver->GenerateEvents();
	/*
	for( int keyCode = 1; keyCode < MOUSE_AFTER_LAST_BUTTON; ++keyCode )
	{
		if( keyCode == ALLEGRO_KEY_MAX )
			keyCode = MOUSE_FIRST_BUTTON;
		
		if( isKeyPressed( keyCode ) )
		{
			stringToEnter->Update( keyCode, ACTION_STRING_TO_ENTER_PRESSED );
			if( eventResponser )
			{
				eventResponser->KeyPressedEvent( keyCode );
			}
		}
		else if( isKeyReleased( keyCode ) )
		{
			if( eventResponser )
			{
				eventResponser->KeyReleasedEvent( keyCode );
			}
		}
		else if( isKeyStillDown( keyCode ) )
		{
			stringToEnter->Update( keyCode, ACTION_STRING_TO_ENTER_HOLDED );
			if( eventResponser )
			{
				eventResponser->KeyHoldedEvent( keyCode );
			}
		}
	}
	
	if( GetMousedX() || GetMousedY() || GetMouseWheelDelta() )
	{
		if( eventResponser )
		{
			eventResponser->MouseMoveEvent( GetMouseX(), GetMouseY(), GetMouseWheelPos(), GetMousedX(), GetMousedY(), GetMouseWheelDelta() );
		}
	}
	*/
	
	eventsTime.SubscribeEnd();
}

EventResponser * BasicWindow::GetEventResponser()
{
	return eventResponser;
}

StringToEnter * BasicWindow::GetStringToEnterObject()
{
	return stringToEnter;
}

BasicWindow::BasicWindow() :
	useParallelThreadToDraw(false)
{
	device = NULL;
	videoDriver = NULL;
	sceneManager = NULL;
	gui = NULL;
	
	quitWhenPossible = false;
	
	deltaTime = 0.01;
	
	lockMouse = false;
	
	eventResponser = NULL;
	stringToEnter = new StringToEnter;
	
	useParallelThreadToDraw = false;
}

BasicWindow::~BasicWindow()
{
	Destroy();
	delete stringToEnter;
	stringToEnter = NULL;
}

#endif

