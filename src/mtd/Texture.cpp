
#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <Texture.h>

unsigned int Texture::OpenGLtextureInit( ALLEGRO_BITMAP * bmap )
{
	if( bmap )
	{
		unsigned int tid = al_get_opengl_texture( bmap );
		if( tid )
		{
			glEnable( GL_TEXTURE_2D );
			
			glBindTexture( GL_TEXTURE_2D, tid );
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mode&Texture::MIPMAP) ? GL_LINEAR_MIPMAP_LINEAR : ( (mode&Texture::LINEAR) ? GL_LINEAR : GL_NEAREST ) );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (mode&Texture::LINEAR) ? GL_LINEAR : GL_NEAREST );
			
			glBindTexture( GL_TEXTURE_2D, 0 );
			
			return tid;
		}
		else
		{
			std::cerr << "\n Error: Can not create OpenGL texture object";
		}
	}
	else
	{
		fprintf( stderr, "\n NULL bitmap pointer passed to OpenGL initialization" );
	}
	return 0;
}

bool Texture::Load( std::string file, int mode )
{
	Destroy();
	this->file = file;
	this->mode = mode;
	al_set_new_bitmap_flags( ( (mode&Texture::LINEAR) ? (ALLEGRO_MIN_LINEAR|ALLEGRO_MAG_LINEAR) : 0 ) | ( (mode&Texture::MIPMAP) ? (ALLEGRO_MIPMAP) : 0 ) );
	bitmap = al_load_bitmap( file.c_str() );
	if( bitmap )
	{
		textureID = OpenGLtextureInit( bitmap );
	}
	else
	{
		std::cerr << "\n Error: Can not load texture";
	}
	return bitmap != NULL;
}

int Texture::GetWidth()
{
	return ( bitmap ? float( al_get_bitmap_width( bitmap ) ) : 0.0 );
}

int Texture::GetHeight()
{
	return ( bitmap ? float( al_get_bitmap_height( bitmap ) ) : 0.0 );
}

ALLEGRO_BITMAP * Texture::GetBitmapPtr()
{
	return bitmap;
}

void Texture::Use( int id )
{
	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 + (id&31) );
	glBindTexture( GL_TEXTURE_2D, textureID );
}

void Texture::Destroy()
{
	file = "";
	if( bitmap )
		al_destroy_bitmap( bitmap );
	textureID = 0;
	bitmap = NULL;
	mode = 0;
}

Texture::Texture()
{
	mode = 0;
	bitmap = NULL;
	textureID = 0;
}

Texture::~Texture()
{
	Destroy();
}

#endif

