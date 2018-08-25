
#ifndef TEXT_PRINTER_CPP
#define TEXT_PRINTER_CPP

#include <TextPrinter.h>
#include <BasicWindow.h>

void TextPrinter::SetBasicWindow( BasicWindow * basicWindow )
{
	this->basicWindow = basicWindow;
}

int TextPrinter::GetMaxBorderWidth()
{
	return ( al_get_bitmap_width( al_get_backbuffer( basicWindow->GetDisplay() ) ) / basicWindow->GetFontWidth() ) - 1;
}

int TextPrinter::GetMaxBorderHeight()
{
	return ( al_get_bitmap_height( al_get_backbuffer( basicWindow->GetDisplay() ) ) / basicWindow->GetFontHeight() ) - 1;
}

void TextPrinter::CorrectCursorPosition()
{
	if( currentCursorX < leftWorkspaceBorder )
	{
		currentCursorX = leftWorkspaceBorder;
	}
	
	if( currentCursorY < upWorkspaceBorder )
	{
		currentCursorY = upWorkspaceBorder;
	}
	
	if( currentCursorX > rightWorkspaceBorder )
	{
		currentCursorX = leftWorkspaceBorder;
		++currentCursorY;
	}
	
	if( currentCursorY > downWorkspaceBorder )
	{
		currentCursorY = downWorkspaceBorder;
	}
}

void TextPrinter::CorrectWorkspaceBorder()
{
	if( leftWorkspaceBorder < 0 )
	{
		leftWorkspaceBorder = 0;
	}
	
	if( upWorkspaceBorder < 0 )
	{
		upWorkspaceBorder = 0;
	}
	
	if( leftWorkspaceBorder > GetMaxBorderWidth()-2 )
	{
		leftWorkspaceBorder = GetMaxBorderWidth()-2;
	}
	
	if( upWorkspaceBorder > GetMaxBorderHeight()-2 )
	{
		upWorkspaceBorder = GetMaxBorderHeight()-2;
	}
	
	if( rightWorkspaceBorder < leftWorkspaceBorder+2 )
	{
		rightWorkspaceBorder = leftWorkspaceBorder+2;
	}
	
	if( downWorkspaceBorder < upWorkspaceBorder+2 )
	{
		downWorkspaceBorder = upWorkspaceBorder+2;
	}
}

void TextPrinter::SetColor( ALLEGRO_COLOR color )
{
	currentTextColor = color;
}

void TextPrinter::Print( const std::string & str )
{
	Print( str.c_str() );
}

void TextPrinter::Print( const char * str )
{
	char * ptr = (char*)str;
	char sign[2];
	sign[1] = 0;
	while( *ptr )
	{
		switch( *ptr )
		{
		case '\t':
			currentCursorX = ( ( ( ( currentCursorX - leftWorkspaceBorder ) / 4 ) + 1 ) * 4 ) + leftWorkspaceBorder;
			break;
			
		case '\n':
			++currentCursorY;
			currentCursorX = 0;
			break;
			
		default:
			*sign = *ptr;
			al_draw_text( basicWindow->GetFont(), currentTextColor, currentCursorX * basicWindow->GetFontWidth(), currentCursorY * basicWindow->GetFontHeight(), ALLEGRO_ALIGN_LEFT, sign );	// change 8 to font size
			++currentCursorX;
		}
		
		++ptr;
		CorrectCursorPosition();
	}
}

void TextPrinter::Print( const char val )
{
	char str[64];
	sprintf( str, "%c", val );
	Print( str );
}

void TextPrinter::Print( const unsigned char val )
{
	char str[64];
	sprintf( str, "%c", val );
	Print( str );
}

void TextPrinter::Print( const short val )
{
	char str[64];
	sprintf( str, "%i", (int)val );
	Print( str );
}

void TextPrinter::Print( const unsigned short val )
{
	char str[64];
	sprintf( str, "%u", (unsigned int)val );
	Print( str );
}

void TextPrinter::Print( const int val )
{
	char str[64];
	sprintf( str, "%i", val );
	Print( str );
}

void TextPrinter::Print( const unsigned int val )
{
	char str[64];
	sprintf( str, "%u", val );
	Print( str );
}

void TextPrinter::Print( const long long int val )
{
	char str[64];
	sprintf( str, "%lli", val );
	Print( str );
}

void TextPrinter::Print( const unsigned long long int val )
{
	char str[64];
	sprintf( str, "%llu", val );
	Print( str );
}

void TextPrinter::Print( const float val )
{
	char str[64];
	sprintf( str, "%f", val );
	Print( str );
}

void TextPrinter::Print( const double val )
{
	Print( (float)val );
}

void TextPrinter::Goto( int x, int y )
{
	currentCursorX = x;
	currentCursorY = y;
	CorrectCursorPosition();
}

void TextPrinter::SetWorkSpace( int lx, int ly, int hx, int hy )
{
	leftWorkspaceBorder = lx;
	rightWorkspaceBorder = hx;
	upWorkspaceBorder = ly;
	downWorkspaceBorder = hy;
	CorrectWorkspaceBorder();
	CorrectCursorPosition();
}

void TextPrinter::ClearWorkspaceBorders()
{
	leftWorkspaceBorder = 0;
	upWorkspaceBorder = 0;
	rightWorkspaceBorder = GetMaxBorderWidth();
	downWorkspaceBorder = GetMaxBorderHeight();
}


TextPrinter::TextPrinter()
{
	basicWindow = NULL;
	currentTextColor = al_map_rgb(255,255,255);
	currentCursorX = 0;
	currentCursorY = 0;
	leftWorkspaceBorder = 0;
	rightWorkspaceBorder = 100;
	upWorkspaceBorder = 0;
	downWorkspaceBorder = 100;
}

TextPrinter::~TextPrinter()
{
	basicWindow = NULL;
	currentTextColor = al_map_rgb(0,0,0);
	currentCursorX = 0;
	currentCursorY = 0;
	leftWorkspaceBorder = 0;
	rightWorkspaceBorder = 0;
	upWorkspaceBorder = 0;
	downWorkspaceBorder = 0;
}

#endif

