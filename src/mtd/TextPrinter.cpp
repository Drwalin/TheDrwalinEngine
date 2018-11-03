
#ifndef TEXT_PRINTER_CPP
#define TEXT_PRINTER_CPP

#include <TextPrinter.h>
#include <BasicWindow.h>
#include <Debug.h>

void TextPrinter::Flush()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.1f );
	
	vbo->Generate( NULL, GL_POINTS );
	
	if( vbo->IsGenerated() )
	{
		glBindVertexArray( vbo->vaoID );
		glBindBuffer( GL_ARRAY_BUFFER, vbo->vboID );
		
		//if( shader.good()? )
		{
			if( vbo->arrayOfShaderLocations.size() < 4 )
			{
				vbo->arrayOfShaderLocations.resize( 4 );
				vbo->arrayOfShaderLocations[0] = -1;
				vbo->arrayOfShaderLocations[1] = -1;
				vbo->arrayOfShaderLocations[2] = -1;
				vbo->arrayOfShaderLocations[3] = -1;
			}
			
			if( vbo->arrayOfShaderLocations[1] < 0 )
				vbo->arrayOfShaderLocations[1] = shader->GetAttributeLocation( "position" );
			vbo->SetAttribPointer( vbo->arrayOfShaderLocations[1], 2, GL_FLOAT, false, 0 );
			
			if( vbo->arrayOfShaderLocations[2] < 0 )
				vbo->arrayOfShaderLocations[2] = shader->GetAttributeLocation( "sTexCoord" );
			vbo->SetAttribPointer( vbo->arrayOfShaderLocations[2], 2, GL_FLOAT, false, 2 * sizeof(float) );
			
			if( vbo->arrayOfShaderLocations[3] < 0 )
				vbo->arrayOfShaderLocations[3] = shader->GetAttributeLocation( "sColor" );
			vbo->SetAttribPointer( vbo->arrayOfShaderLocations[3], 4, GL_UNSIGNED_BYTE, false, 2 * sizeof(float) + 2 * sizeof(float) );
			
			shader->Use();
			
			font->Use( 0 );
			
			if( vbo->arrayOfShaderLocations[0] < 0 )
				vbo->arrayOfShaderLocations[0] = shader->GetUniformLocation( ( std::string("texture0") ).c_str() );
			shader->SetInt( vbo->arrayOfShaderLocations[0], 0 );
		}
		
		glDrawArrays( GL_TRIANGLES, 0, vbo->vertices.size() );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glUseProgram( 0 );
	}
	
	vbo->Destroy();
	vbo->ClearVertices();
	
	if( basicWindow )
	{
		
		windowX = basicWindow->GetWidth() / 18.0f;
		windowY = basicWindow->GetHeight() / 32.0f;
		
		/*
		windowX = 100.0f;
		windowY = 75.0f;
		*/
	}
	else
	{
		DEBUG("No basicWindow reference in TextPrinter")
	}
}

void TextPrinter::Init()
{
	vbo = new VBO;
	font = new Texture;
	shader = new Shader;
	DEBUG(0)
	font->Load( "media\\Textures\\font.png", 0 );
	DEBUG(1)
	shader->Load( "media\\Shaders\\font.vs", /*"media\\Shaders\\font.gs"*/"", "media\\Shaders\\font.fs" );
	DEBUG(2)
}

void TextPrinter::AddCharacter( const float x, const float y, const unsigned char sign )
{
	struct Temp__d
	{
		float pos[2];
		float tex[2];
		unsigned char color[4];
	} temp;
	
	vbo->SetVertexSize( sizeof(Temp__d) );
	
	
	*((ALLEGRO_COLOR*)&temp.color) = currentTextColor;
	al_unmap_rgba( currentTextColor, temp.color, temp.color+1, temp.color+2, temp.color+3 );
	
	
	float a, b;
	a = float(sign & 31) / 32.0f;
	b = float(sign >> 5) / 8.0f;
	
	temp.pos[0] = x;
	temp.pos[1] = y;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a;
	temp.tex[1] = b;
	
	vbo->AddVertex( &temp );
	
	
	temp.pos[0] = x+1;
	temp.pos[1] = y;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a+(1.0f/32.0f);
	temp.tex[1] = b;
	
	vbo->AddVertex( &temp );
	
	
	temp.pos[0] = x;
	temp.pos[1] = y+1;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a;
	temp.tex[1] = b+(1.0f/8.0f);
	
	vbo->AddVertex( &temp );
	
	
	
	temp.pos[0] = x+1;
	temp.pos[1] = y;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a+(1.0f/32.0f);
	temp.tex[1] = b;
	
	vbo->AddVertex( &temp );
	
	
	temp.pos[0] = x;
	temp.pos[1] = y+1;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a;
	temp.tex[1] = b+(1.0f/8.0f);
	
	vbo->AddVertex( &temp );
	
	temp.pos[0] = x+1;
	temp.pos[1] = y+1;
	temp.pos[1] *= 2.0f;
	temp.pos[0] /= windowX;
	temp.pos[1] /= windowY;
	temp.pos[1] = 1.0f - temp.pos[1];
	temp.pos[0] -= 1.0f;
	
	temp.tex[0] = a+(1.0f/32.0f);
	temp.tex[1] = b+(1.0f/8.0f);
	
	vbo->AddVertex( &temp );
}

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
			AddCharacter( currentCursorX, currentCursorY, *ptr );
			
			
			//*sign = *ptr;
			//al_draw_text( basicWindow->GetFont(), currentTextColor, currentCursorX * basicWindow->GetFontWidth(), currentCursorY * basicWindow->GetFontHeight(), ALLEGRO_ALIGN_LEFT, sign );	// change 8 to font size
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
	windowX = 100.0f;
	windowY = 75.0f;
	vbo = NULL;
	font = NULL;
	shader = NULL;
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
	if( vbo )
		delete vbo;
	if( shader )
		delete shader;
	if( font )
		delete font;
	vbo = NULL;
	font = NULL;
	shader = NULL;
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

