
#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <Texture.h>

bool Texture::Load( std::string file, int mode )
{
	Destroy();
	this->file = file;
	this->mode = mode;
	al_set_new_bitmap_flags( ( (mode&Texture::LINEAR) ? (ALLEGRO_MIN_LINEAR|ALLEGRO_MAG_LINEAR) : 0 ) | ( (mode&Texture::MIPMAP) ? (ALLEGRO_MIPMAP) : 0 ) );
	bitmap = al_load_bitmap( file.c_str() );
	if( bitmap )
	{
		textureID = al_get_opengl_texture( bitmap );
		if( textureID )
		{
			glBindTexture( GL_TEXTURE_2D, textureID );
			
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mode&Texture::LINEAR) ? GL_LINEAR : GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (mode&Texture::LINEAR) ? GL_LINEAR : GL_NEAREST );
			
			//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
			
			if( mode & Texture::MIPMAP )
				glGenerateMipmap( GL_TEXTURE_2D );
			
			
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
		else
		{
			std::cerr << "\n Error: Can not create OpenGL texture object";
		}
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
	//std::cerr << "\n Texture::Use() is not in use. Please use: Texture::GetBitmapPtr() to pass as argument to al_draw_*_prim";
}

void Texture::Destroy()
{
	file = "";
	if( bitmap )
		al_destroy_bitmap( bitmap );
	bitmap = NULL;
	mode = 0;
	textureID = 0;
}

Texture::Texture( SmartPtr<Texture> other )
{
	if( other )
	{
		this->file = other->file;
		this->mode = other->mode;
		if( other->bitmap )
		{
			al_set_new_bitmap_flags( ( (mode&Texture::LINEAR) ? (ALLEGRO_MIN_LINEAR|ALLEGRO_MAG_LINEAR) : 0 ) | ( (mode&Texture::MIPMAP) ? (ALLEGRO_MIPMAP) : 0 ) );
			bitmap = al_clone_bitmap( other->bitmap );
			
			textureID = al_get_opengl_texture( bitmap );
			if( textureID )
			{
				glBindTexture( GL_TEXTURE_2D, textureID );
				
				
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
				
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mode&Texture::LINEAR) ? GL_LINEAR : GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (mode&Texture::LINEAR) ? GL_LINEAR : GL_NEAREST );
				
				
				//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
				
				
				if( mode & Texture::MIPMAP )
					glGenerateMipmap( GL_TEXTURE_2D );
				
				
				glBindTexture( GL_TEXTURE_2D, 0 );
			}
			else
			{
				std::cerr << "\n Error: Can not create OpenGL texture object";
			}
		}
		else
			bitmap = NULL;
	}
	else
	{
		mode = Texture::MIPMAP | Texture::LINEAR;
		bitmap = NULL;
		std::cerr << "\n Error: Can not copy bitmap";
	}
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

