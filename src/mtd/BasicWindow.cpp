
#ifndef BASIC_WINDOW_CPP
#define BASIC_WINDOW_CPP

#include <BasicWindow.h>
#include <Debug.h>

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
	al_hide_mouse_cursor( display );
}

void BasicWindow::ShowMouse()
{
	al_show_mouse_cursor( display );
}

float BasicWindow::GetDeltaTime()
{
	return deltaTime;
}

ALLEGRO_FONT * BasicWindow::GetFont()
{
	return font;
}

int BasicWindow::GetFontWidth()
{
	return 8;
}

int BasicWindow::GetFontHeight()
{
	return 9;
}

ALLEGRO_BITMAP * BasicWindow::GetDisplayBitmap()
{
	return al_get_backbuffer( display );
}

ALLEGRO_DISPLAY * BasicWindow::GetDisplay()
{
	return display;
}

void BasicWindow::Use3DSpace()
{
	al_set_target_bitmap( al_get_backbuffer( display ) );
	al_set_render_state( ALLEGRO_DEPTH_TEST, 1 );
	al_set_render_state( ALLEGRO_DEPTH_FUNCTION, ALLEGRO_RENDER_LESS );
	al_set_blender( ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA );
	al_use_projection_transform( &projection3DTransform );
	
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	al_use_transform( &transform );
}

void BasicWindow::Use2DSpace()
{
	al_set_target_bitmap( al_get_backbuffer( display ) );
	al_set_render_state( ALLEGRO_DEPTH_TEST, 0 );
	al_set_blender( ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA );
	al_use_projection_transform( &projection2DTransform );
	
	ALLEGRO_TRANSFORM transform;
	al_identity_transform( &transform );
	al_use_transform( &transform );
}

void BasicWindow::LoadIconFromFile( const char * iconFile )
{
	if( iconFile )
	{
		icon = al_load_bitmap( iconFile );
		al_set_display_icon( display, icon );
	}
	else
	{
		icon = NULL;
	}
}

bool BasicWindow::CreateDisplay( const char * windowName, int width, int height, bool fullscreen )
{
//	al_set_new_display_flags( ALLEGRO_RESIZABLE );
	al_set_new_display_option( ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST );
	al_set_new_display_option( ALLEGRO_SUPPORT_NPOT_BITMAP, 0, ALLEGRO_REQUIRE) ;
	al_set_new_window_position( 350, 200 );
//	al_set_new_display_option( ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST );
	
	if( fullscreen )
		al_set_new_display_flags( ALLEGRO_FULLSCREEN_WINDOW );
	
	display = al_create_display( width, height );
	if( display == NULL )
	{
		printf( "\n Error: faild to create display in Window::Init " );
		return false;
	}
	
	al_set_window_title( display, windowName );
	
	return true;
}

bool BasicWindow::InitFont()
{
	font = al_create_builtin_font();
	if( font == NULL )
	{
		std::cerr << "\n Error: faild to create builtin font in Window::Init ";
		return false;
	}
	return true;
}

void BasicWindow::InitTransforms()
{
	al_identity_transform( &projection2DTransform );
	al_identity_transform( &projection3DTransform );
	
	al_copy_transform( &projection2DTransform, al_get_current_projection_transform() );
	
	CreatePerspectiveTransform( 0.1, 100000.0 );
}

bool BasicWindow::InitAllegro()
{
	if( !al_init() || !al_init_image_addon() || !al_init_primitives_addon() || !al_init_font_addon() || !al_install_keyboard() || !al_install_mouse() )
	{
		std::cerr << "\n Error: one of al_init* failed in Window::Init ";
		return false;
	}
	return true;
}

bool BasicWindow::Init( const char * windowName, const char * iconFile, int width, int height, bool fullscreen )
{
	if( !InitAllegro() )
		return false;
	
	if( !InitFont() )
		return false;
	
	if( !CreateDisplay( windowName, width, height, fullscreen ) )
		return false;
	
	LoadIconFromFile( iconFile );
	
	InitTransforms();
	
	stringToEnter->SetBasicWindow( this );
	
	return true;
}

void BasicWindow::Destroy()
{
	if( display )
		al_destroy_display( display );
	if( font )
		al_destroy_font( font );
	if( icon )
		al_destroy_bitmap( icon );
	
	quitWhenPossible = false;
	
	icon = NULL;
	display = NULL;
	font = NULL;
	zNear = 0.0;
	zFar = 0.0;
	deltaTime = 0.0;
	
	lockMouse = false;
	
	eventResponser = NULL;
}

void BasicWindow::CreatePerspectiveTransform( float zNear, float zFar )
{
	this->zNear = zNear;
	this->zFar = zFar;
	float aspect_ratio = (float)al_get_bitmap_height( al_get_backbuffer( display ) ) / (float)al_get_bitmap_width( al_get_backbuffer( display ) );
	al_identity_transform( &projection3DTransform );
	al_perspective_transform( &projection3DTransform, -zNear, aspect_ratio * zNear, zNear, zNear, -aspect_ratio * zNear, zFar );
}

void BasicWindow::OneLoopFullTick()
{
	static float beginTime = al_get_time();
	
	deltaTime = al_get_time() - beginTime;
	beginTime = al_get_time();
	
	if( deltaTime < 0.001 )
		deltaTime = 0.001;
	else if( deltaTime > 0.3 )
		deltaTime = 0.3;
	
	AlTick();
	
	if( al_get_time() - beginTime < 1.0/60.0 )
		al_rest( (1.0/60.0) - (al_get_time() - beginTime) );
}

void BasicWindow::BeginLoop()
{
	quitWhenPossible = false;
	while( !quitWhenPossible )
		OneLoopFullTick();
}

void BasicWindow::QueueQuit()
{
	quitWhenPossible = true;
}

void BasicWindow::AlDraw()
{
	Use2DSpace();
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	
	Use3DSpace();
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	al_clear_depth_buffer( zFar );
	
	Draw3D();
	
	Use2DSpace();
	Draw2D();
	
	al_flip_display();
	
}


extern "C" ALLEGRO_MOUSE_STATE mouseLastFrame;
void BasicWindow::AlTick()
{
	UpdateKeyboard();
	
	if( lockMouse )
	{
		al_set_mouse_xy( display, al_get_bitmap_width(al_get_backbuffer(display)) / 2, al_get_bitmap_height(al_get_backbuffer(display)) / 2 );
		mouseLastFrame.x = al_get_bitmap_width(al_get_backbuffer(display)) / 2;
		mouseLastFrame.y = al_get_bitmap_height(al_get_backbuffer(display)) / 2;
	}
	
	GenerateEvents();
	
	Tick( deltaTime );
	
	AlDraw();
}

void BasicWindow::GenerateEvents()
{
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
}

void BasicWindow::SetEventResponser( EventResponser * eventResponser )
{
	this->eventResponser = eventResponser;
	this->eventResponser->SetBasicWindow( this );
}

EventResponser * BasicWindow::GetEventResponser()
{
	return eventResponser;
}

BasicWindow::BasicWindow()
{
	quitWhenPossible = false;
	
	ALLEGRO_TRANSFORM projection2DTransform;
	ALLEGRO_TRANSFORM projection3DTransform;
	
	icon = NULL;
	display = NULL;
	font = NULL;
	zNear = 0.01;
	zFar = 1000.0;
	deltaTime = 0.01;;
	
	lockMouse = false;
	
	eventResponser = NULL;
	stringToEnter = new StringToEnter;
}

BasicWindow::~BasicWindow()
{
	Destroy();
	delete stringToEnter;
}

#endif

